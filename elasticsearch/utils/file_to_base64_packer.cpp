#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <filesystem>
#include <string>

#include <txml/txml.hpp>
#include "elasticsearch/utils/file_to_base64_packer.hpp"


namespace  elasticsearch
{
namespace utils
{
packer::packer(const std::filesystem::path &file_path)
{
    packer::pack(file_path);
}

void packer::pack(const std::filesystem::path &path_to_pack)
{
    original_data_path.reset(new elasticsearch::common_model::OriginalPath(std::filesystem::absolute(path_to_pack)));

    std::unique_ptr<FILE, decltype(&fclose)> packed_file (fopen(path_to_pack.c_str(), "rb"), fclose);
    if (!packed_file)
    {
        throw std::runtime_error(std::string("cannot open filepath: ") + path_to_pack.c_str());
    }
    fseek(packed_file.get(), 0, SEEK_END);
    long file_size = ftell(packed_file.get());
    fseek(packed_file.get(), 0, SEEK_SET);

    std::vector<uint8_t> file_data(file_size, 0);
    size_t total_bytes_read = 0;
    size_t bytes_read = 0;
    do {
        bytes_read = fread(file_data.data() + total_bytes_read,
                           1, file_size - total_bytes_read,
                           packed_file.get());
        if (bytes_read > 0)
        {
            total_bytes_read += bytes_read;
        }
    } while(bytes_read != 0 or errno == EINTR);
    data_ptr = std::make_shared<elasticsearch::common_model::BinaryBlob>(std::move(file_data));
}

std::shared_ptr<elasticsearch::common_model::BinaryBlob> packer::getBlob() const
{
    return data_ptr;
}

std::shared_ptr<elasticsearch::common_model::OriginalPath> packer::getPath() const
{
    return original_data_path;
}
}
}
