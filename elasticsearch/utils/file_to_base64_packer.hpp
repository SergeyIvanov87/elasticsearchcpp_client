#ifndef ELASTIC_SEARCH_COMMON_PACKER_HPP
#define ELASTIC_SEARCH_COMMON_PACKER_HPP

#include "elasticsearch/interfaces/packer.hpp"

namespace  elasticsearch
{
namespace utils
{
class packer: public elasticsearch::packer_interface
{
public:
    packer(const std::filesystem::path &path_to_pack);

    std::optional<elasticsearch::common_model::BinaryBlob> getBlob() const override;
    std::optional<elasticsearch::common_model::OriginalPath> getPath() const override;
    void pack(const std::filesystem::path &path_to_pack) override;
private:
    std::optional<elasticsearch::common_model::OriginalPath> original_data_path;
    std::optional<elasticsearch::common_model::BinaryBlob> data_ptr;
};
}
}
#endif // ELASTIC_SEARCH_COMMON_PACKER_HPP
