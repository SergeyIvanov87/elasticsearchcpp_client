#ifndef ELASTIC_SEARCH_BOOK_FB2_PACKER_HPP
#define ELASTIC_SEARCH_BOOK_FB2_PACKER_HPP

#include "elasticsearch/interfaces/packer.hpp"

namespace fb2
{
class packer : public elasticsearch::packer_interface
{
public:
    packer(const std::string& file_path);

    std::shared_ptr<elasticsearch::common_model::BinaryBlob> getBlob() const override;
    std::shared_ptr<elasticsearch::common_model::OriginalPath> getPath() const override;
    void pack(const std::filesystem::path &path_to_pack) override;
private:
    std::shared_ptr<elasticsearch::common_model::OriginalPath> original_data_path;
    std::shared_ptr<elasticsearch::common_model::BinaryBlob> data_ptr;
};
}
#endif // ELASTIC_SEARCH_BOOK_FB2_PACKER_HPP
