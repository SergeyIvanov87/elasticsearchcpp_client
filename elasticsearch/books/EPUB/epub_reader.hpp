#ifndef ELASTIC_SEARCH_BOOK_EPUB_READER_HPP
#define ELASTIC_SEARCH_BOOK_EPUB_READER_HPP

#include <memory>
#include "elasticsearch/books/interfaces/book_packer.hpp"
#include "elasticsearch/books/data_model/model.hpp"

namespace elasticsearch
{
namespace utils
{
class packer;
}
}
namespace EPUB
{
class Package;
class reader : public elasticsearch::book::packer_interface_aggregator
{
public:
    reader(const std::string& file_path);
    ~reader();

    std::shared_ptr<Package> getOPF() const;
    std::shared_ptr<elasticsearch::common_model::BinaryBlob> getBlob() const override;
    std::shared_ptr<elasticsearch::common_model::OriginalPath> getPath() const override;

    std::shared_ptr<elasticsearch::book::model::data> to_model(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    std::shared_ptr<elasticsearch::book::model::data> to_model(txml::StdoutTracer tracer = txml::StdoutTracer()) const override;

    nlohmann::json to_json(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    nlohmann::json to_json(txml::StdoutTracer tracer) const override;
private:
    void pack(const std::filesystem::path &path_to_pack) override;

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<elasticsearch::book::model::data> to_model_impl(Tracer tracer = Tracer()) const;

    template<class Tracer = txml::EmptyTracer>
    nlohmann::json to_json_impl(Tracer tracer = Tracer()) const;

    std::shared_ptr<Package> open_packaging_format_ptr;
    std::unique_ptr<elasticsearch::utils::packer> packer_impl;
};
}
#endif // ELASTIC_SEARCH_BOOK_EPUB_READER_HPP
