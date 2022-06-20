#include "elasticsearch/books/book_reader.hpp"

#include <txml/applications/fb2/fb2_fwd.h>
#include "elasticsearch/books/FB2/fb2_reader.hpp"
#include "elasticsearch/books/EPUB/epub_reader.hpp"

namespace elasticsearch
{
namespace book
{
reader::reader(const std::string& file_path)
try {
    try {
        impl.reset(new fb2::reader(file_path));
    } catch (...) {
        try {
            impl.reset(new EPUB::reader(file_path));
        } catch (...) {
            throw;
        };
    }
} catch (...) {
    throw std::runtime_error("Unsupported book format by path: " + file_path);
}

std::optional<elasticsearch::common_model::BinaryBlob> reader::getBlob() const
{
    return impl->getBlob();
}

std::optional<elasticsearch::common_model::OriginalPath> reader::getPath() const
{
    return impl->getPath();
}

void reader::pack(const std::filesystem::path &path_to_pack)
{
    impl->pack(path_to_pack);
}

std::optional<elasticsearch::book::model::data> reader::to_model(txml::EmptyTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::book::packer_interface<txml::EmptyTracer>>(impl)->to_model(std::move(tracer));
}
std::optional<elasticsearch::book::model::data> reader::to_model(txml::StdoutTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::book::packer_interface<txml::StdoutTracer>>(impl)->to_model(std::move(tracer));
}

nlohmann::json reader::to_json(txml::EmptyTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::book::packer_interface<txml::EmptyTracer>>(impl)->to_json(std::move(tracer));
}
nlohmann::json reader::to_json(txml::StdoutTracer tracer) const
{
    return std::static_pointer_cast<elasticsearch::book::packer_interface<txml::StdoutTracer>>(impl)->to_json(std::move(tracer));
}
}
}
