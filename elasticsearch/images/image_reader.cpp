#include <txml/txml.hpp>

#include "elasticsearch/images/image_reader.hpp"
#include "elasticsearch/images/JPG/reader.hpp"

namespace elasticsearch
{
namespace image
{
reader::reader(const std::string& file_path)
try
{
    try {
        impl.reset(new JPG::reader(file_path));
    } catch (...) {
        throw;
    }
} catch (...) {
    throw std::runtime_error("Unsupported image format by path: " + file_path);
}

std::shared_ptr<elasticsearch::common_model::BinaryBlob> reader::getBlob() const
{
    return impl->getBlob();
}

std::shared_ptr<elasticsearch::common_model::OriginalPath> reader::getPath() const
{
    return impl->getPath();
}

void reader::pack(const std::filesystem::path &path_to_pack)
{
    impl->pack(path_to_pack);
}

std::shared_ptr<elasticsearch::image::model::data> reader::to_model(txml::EmptyTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::image::packer_interface<txml::EmptyTracer>>(impl)->to_model(std::move(tracer));
}
std::shared_ptr<elasticsearch::image::model::data> reader::to_model(txml::StdoutTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::image::packer_interface<txml::StdoutTracer>>(impl)->to_model(std::move(tracer));
}

nlohmann::json reader::to_json(txml::EmptyTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::image::packer_interface<txml::EmptyTracer>>(impl)->to_json(std::move(tracer));
}
nlohmann::json reader::to_json(txml::StdoutTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::image::packer_interface<txml::StdoutTracer>>(impl)->to_json(std::move(tracer));
}
}
}
