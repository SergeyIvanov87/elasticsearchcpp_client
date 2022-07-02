#ifndef ELASTIC_SEARCH_BOOK_FB2_READER_HPP
#define ELASTIC_SEARCH_BOOK_FB2_READER_HPP

#include <memory>

#include "elasticsearch/books/FB2/model/ShortFictionBook.hpp"
#include "elasticsearch/books/interfaces/book_packer.hpp"

namespace fb2
{
class ShortFictionBook;
class packer;
class reader : public elasticsearch::book::packer_interface_aggregator
{
public:
    reader(const std::string& file_path);
    ~reader();

    const ShortFictionBook &getBook() const;
    const elasticsearch::common_model::BinaryBlob &getBlob() const override;
    const elasticsearch::common_model::OriginalPath &getPath() const override;

    elasticsearch::book::model::data to_model(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    elasticsearch::book::model::data to_model(txml::StdoutTracer tracer = txml::StdoutTracer()) const override;

    nlohmann::json to_json(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    nlohmann::json to_json(txml::StdoutTracer tracer) const override;
private:
    void pack(const std::filesystem::path &path_to_pack) override;

    template<class Tracer = txml::EmptyTracer>
    elasticsearch::book::model::data to_model_impl(Tracer tracer = Tracer()) const;

    template<class Tracer = txml::EmptyTracer>
    nlohmann::json to_json_impl(Tracer tracer = Tracer()) const;

    ShortFictionBook book_ptr;
    std::unique_ptr<packer> packer_impl;
};
}
#endif // ELASTIC_SEARCH_BOOK_FB2_READER_HPP
