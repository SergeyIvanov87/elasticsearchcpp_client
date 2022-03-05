#ifndef ELASTIC_SEARCH_COMMON_UNPACKER_HPP
#define ELASTIC_SEARCH_COMMON_UNPACKER_HPP

#include <filesystem>
#include <memory>
#include <string>
#include "elasticsearch/common_model/BinaryBlob.h"
#include "elasticsearch/common_model/OriginalPath.h"

namespace  elasticsearch
{
namespace utils
{
class unpacker
{
public:
    unpacker(const elasticsearch::common_model::BinaryBlob& blob,
             const std::filesystem::path &filepath);
    unpacker(const elasticsearch::common_model::BinaryBlob& blob,
             const elasticsearch::common_model::OriginalPath& restore_path);

    const std::string& getFilePath() const;
private:
    std::string restored_file_path;
};
}
}
#endif // ELASTIC_SEARCH_COMMON_UNPACKER_HPP
