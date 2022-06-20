#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <txml/txml.hpp>

#include "elasticsearch/utils/zutils.hpp"
#include "elasticsearch/utils/base64_to_file_unpacker.hpp"

namespace  elasticsearch
{
namespace utils
{
unpacker::unpacker(const elasticsearch::common_model::BinaryBlob& blob,
                   const std::filesystem::path &file_name) :
    restored_file_path(file_name)
{
    // decode base64 in compressed file data
    std::vector<uint8_t> binary_data = blob.getEncodedValue();
    if (binary_data.empty())
    {
        throw std::runtime_error("nothing to unpack");
    }

    // create result file
    utils::fd_guard unpacked_fd(utils::fd_guard::INVALID);
    try
    {
        if (restored_file_path.empty())
        {
            char tmp_name[7] {"XXXXXX"};
            unpacked_fd = utils::fd_guard(mkstemp(tmp_name));
            if (unpacked_fd == -1)
            {
                throw std::runtime_error("cannot create tmp unpacked file, error: " +
                                         std::to_string(errno) + strerror(errno));
            }
            restored_file_path = tmp_name;
        }
        else
        {
            unpacked_fd = utils::fd_guard(open(restored_file_path.c_str(), O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP));
            if (unpacked_fd == -1)
            {
                throw std::runtime_error("cannot create unpacked file, path: " + restored_file_path +
                                         ", error: " + std::to_string(errno) + strerror(errno));
            }
        }
    }
    catch (...)
    {
        throw;
    }

    size_t written_bytes = 0;
    do
    {
        ssize_t ret = write(unpacked_fd, binary_data.data() + written_bytes, binary_data.size() - written_bytes);
        if (ret == -1)
        {
            if (errno != EINTR)
            {
                break;
            }
            continue;
        }
        written_bytes += ret;
    } while(written_bytes != binary_data.size());
    if(written_bytes != binary_data.size())
    {
        unlink(restored_file_path.c_str());
        throw std::runtime_error("Cannot write unpacked file, error: " + std::to_string(errno) +
                                " (" + strerror(errno) + ")");
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
}
