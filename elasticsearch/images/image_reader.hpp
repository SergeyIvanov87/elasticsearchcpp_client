#ifndef ELASTIC_SEARCH_IMAGE_READER_HPP
#define ELASTIC_SEARCH_IMAGE_READER_HPP

#include <memory>
#include "elasticsearch/images/interfaces/image_packer.hpp"
#include "elasticsearch/images/data_model/model.hpp"

namespace elasticsearch
{
namespace image
{
class reader : public elasticsearch::image::packer_interface_aggregator
{
public:
    reader(const std::string& file_path);

    std::optional<elasticsearch::common_model::BinaryBlob> getBlob() const override;
    std::optional<elasticsearch::common_model::OriginalPath> getPath() const override;

    std::optional<elasticsearch::image::model::data> to_model(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    std::optional<elasticsearch::image::model::data> to_model(txml::StdoutTracer tracer = txml::StdoutTracer()) const override;

    nlohmann::json to_json(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    nlohmann::json to_json(txml::StdoutTracer tracer) const override;
private:
    void pack(const std::filesystem::path &path_to_pack) override;

    std::shared_ptr<elasticsearch::image::packer_interface_aggregator> impl;
};
}
}
#endif // ELASTIC_SEARCH_IMAGE_READER_HPP
