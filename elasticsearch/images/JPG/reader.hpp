#ifndef ELASTIC_SEARCH_BOOK_JPG_READER_HPP
#define ELASTIC_SEARCH_BOOK_JPG_READER_HPP

#include <filesystem>
#include <memory>

#include "elasticsearch/images/interfaces/image_packer.hpp"
#include "elasticsearch/images/JPG/model/ExifData.h"

namespace elasticsearch
{
namespace utils
{
class packer;
}
}

namespace JPG
{
class reader : public elasticsearch::image::packer_interface_aggregator
{
public:
    reader(const std::filesystem::path &file_path);
    ~reader();

    std::shared_ptr<ExifData> getImageModel() const;
    std::shared_ptr<elasticsearch::common_model::BinaryBlob> getBlob() const override;
    std::shared_ptr<elasticsearch::common_model::OriginalPath> getPath() const override;

    std::shared_ptr<elasticsearch::image::model::data> to_model(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    std::shared_ptr<elasticsearch::image::model::data> to_model(txml::StdoutTracer tracer = txml::StdoutTracer()) const override;

    nlohmann::json to_json(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    nlohmann::json to_json(txml::StdoutTracer tracer) const override;
private:
    void pack(const std::filesystem::path &path_to_pack) override;

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<elasticsearch::image::model::data> to_model_impl(Tracer tracer = Tracer()) const;

    template<class Tracer = txml::EmptyTracer>
    nlohmann::json to_json_impl(Tracer tracer = Tracer()) const;

    std::unique_ptr<elasticsearch::utils::packer> packer_impl;

    std::shared_ptr<ExifData> image_data;
};
}
#endif // ELASTIC_SEARCH_BOOK_JPG_READER_HPP
