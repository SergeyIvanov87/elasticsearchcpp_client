#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "elasticsearch/utils/zutils.hpp"
#include "elasticsearch/books/FB2/fb2_unpacker.hpp"

namespace fb2
{
unpacker::unpacker(const elasticsearch::common_model::BinaryBlob& blob,
                   const std::string& file_name) :
    restored_file_path(file_name)
{
    // decode base64 in compressed file data
    std::vector<uint8_t> binary_data = blob.getEncodedValue();
    if (binary_data.empty())
    {
        throw std::runtime_error("nothing to restore");
    }

    // create tmp file to gunzip compressed file data
    elasticsearch::utils::fd_guard base64_fd = elasticsearch::utils::make_tmpfd();
    size_t bytes = 0;
    do
    {
        ssize_t ret = write(base64_fd, binary_data.data() + bytes, binary_data.size() - bytes);
        if (ret == -1)
        {
            if (errno != EINTR)
            {
                break;
            }
            continue;
        }
        bytes += ret;
    } while(bytes != binary_data.size());
    if(bytes != binary_data.size())
    {
        throw std::runtime_error("Cannot write temporary compressed file, error: " + std::to_string(errno) +
                                " (" + strerror(errno) + ")");
    }

    // create result FB2 file
    elasticsearch::utils::fd_guard unpacked_fd(elasticsearch::utils::fd_guard::INVALID);
    try
    {
        if (restored_file_path.empty())
        {
            char tmp_name[7] {"XXXXXX"};
            unpacked_fd = elasticsearch::utils::fd_guard(mkstemp(tmp_name));
            if (unpacked_fd == -1)
            {
                throw std::runtime_error("cannot create tmp decompressed file, error: " +
                                         std::to_string(errno) + strerror(errno));
            }
            restored_file_path = tmp_name;
        }
        else
        {
            unpacked_fd = elasticsearch::utils::fd_guard(open(restored_file_path.c_str(), O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP));
            if (unpacked_fd == -1)
            {
                throw std::runtime_error("cannot create decompressed file, path: " + restored_file_path +
                                         ", error: " + std::to_string(errno) + strerror(errno));
            }
        }
    }
    catch (...)
    {
        throw;
    }

    // make decompress data to result FB2 file
    try
    {
        lseek(base64_fd, 0, SEEK_SET);
        elasticsearch::utils::unique_gzFile compressed_file = elasticsearch::utils::create_gzdfile(base64_fd.dup(), "rb");
        gzrewind(compressed_file.get());

        // prepare burst buffer
        binary_data.clear();
        binary_data.resize(std::min<size_t>(binary_data.capacity(), 4096), 0);
        while(!gzeof(compressed_file.get()))
        {
            // uncompress bunch
            int inflated_bytes = gzread(compressed_file.get(), binary_data.data(), binary_data.size());
            if (inflated_bytes == -1)
            {
                int errnum = 0;
                gzerror(compressed_file.get(), &errnum);
                throw std::runtime_error("Cannot read compressed file, errnum: " + std::to_string(errnum));
            }
            int written = 0;
            do
            {
                // write uncompressed bunch into result file
                ssize_t ret = write(unpacked_fd, binary_data.data() + written, inflated_bytes - written);
                if (written == -1)
                {
                    if (errno != EINTR)
                    {
                        break;
                    }
                    continue;
                }
                written += ret;
            } while(written != inflated_bytes);
            if(written != inflated_bytes)
            {
                throw std::runtime_error("Cannot write decompressed file");
            }
        }
    }
    catch (...)
    {
        unlink(restored_file_path.c_str());
        throw;
    }
}

unpacker::unpacker(const elasticsearch::common_model::BinaryBlob& blob,
                   const elasticsearch::common_model::OriginalPath& restore_path) :
    unpacker(blob, restore_path.value())
{
}

const std::string &unpacker::getFilePath() const
{
    return restored_file_path;
}
}
