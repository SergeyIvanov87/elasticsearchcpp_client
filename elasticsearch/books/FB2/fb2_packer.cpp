#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <filesystem>
#include <string>

#include "elasticsearch/utils/zutils.hpp"

#include "elasticsearch/books/FB2/fb2_packer.hpp"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

namespace fb2
{
packer::packer(const std::string& file_path) :
    original_data_path(std::filesystem::absolute(file_path))
{
    packer::pack(file_path);
}

void packer::pack(const std::filesystem::path &path_to_pack)
{
    //gzdopen
    std::unique_ptr<FILE, decltype(&fclose)> unpacked_file (fopen(path_to_pack.c_str(), "rb"), fclose);
    if (!unpacked_file)
    {
        throw std::runtime_error(std::string("cannot open file to compression by path: ") + path_to_pack.c_str());
    }

    // create temporary file for compressing
    elasticsearch::utils::fd_guard packed_fd = elasticsearch::utils::make_tmpfd();
    elasticsearch::utils::unique_gzFile gzfile = elasticsearch::utils::create_gzdfile(packed_fd.dup(), "wb");

    // read book and compress it simultaneosuly
    std::vector<uint8_t> file_data(4096, 0);
    size_t bytes_read = 0;
    size_t total_bytes_deflated = 0;
    do {
        bytes_read = fread(file_data.data(), 1, 4096, unpacked_file.get());
        if (bytes_read > 0)
        {
            int bytes_deflated = 0;
            do
            {
                int ret = gzwrite(gzfile.get(), file_data.data(), bytes_read);
                if (!ret)
                {
                    int errnum = 0;
                    gzerror(gzfile.get(), &errnum);
                    throw std::runtime_error("cannot compress data in gzfile, error: " +
                                 std::to_string(errnum));
                }
                bytes_deflated += ret;
            } while (bytes_deflated != bytes_read);
            total_bytes_deflated += bytes_deflated;
        }
    } while(bytes_read != 0 or errno == EINTR);
    if (total_bytes_deflated == 0 or errno != 0)
    {
        throw std::runtime_error("cannot compress data in gzfile, read error: " +
                                 std::to_string(errno) + " (" + strerror(errno) + ")");
    }
    gzfile.reset();
    total_bytes_deflated = lseek(packed_fd, 0, SEEK_END);

    lseek(packed_fd, 0, SEEK_SET);

    // read from dup and encode in base64
    file_data.resize(total_bytes_deflated, 0);
    size_t read_count = 0;
    ssize_t ret = 0;
    do{
        read_count += ret;
        ssize_t ret = read(packed_fd, file_data.data() + read_count, file_data.size() - read_count);
        if (ret == -1 && errno != EINTR)
        {
            break;
        }
    } while(ret);
    if(errno != 0)
    {
        throw std::runtime_error("cannot read compressed file, read error: " +
                                 std::to_string(errno) + " (" + strerror(errno) + ")");
    }
    data_ptr = std::make_unique<elasticsearch::common_model::BinaryBlob>(std::move(file_data));
}

const elasticsearch::common_model::BinaryBlob& packer::getBlob() const
{
    return *data_ptr;
}

const elasticsearch::common_model::OriginalPath& packer::getPath() const
{
    return original_data_path;
}
}
