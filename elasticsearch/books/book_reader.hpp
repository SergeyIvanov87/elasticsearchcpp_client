#ifndef ELASTIC_SEARCH_BOOK_READER_HPP
#define ELASTIC_SEARCH_BOOK_READER_HPP

#include <memory>
#include "elasticsearch/books/interfaces/book_packer.hpp"
#include "elasticsearch/books/data_model/model.hpp"

namespace elasticsearch
{
namespace book
{
class reader : public elasticsearch::book::packer_interface_aggregator
{
public:
    reader(const std::string& file_path);

    const elasticsearch::common_model::BinaryBlob &getBlob() const override;
    const elasticsearch::common_model::OriginalPath &getPath() const override;

    elasticsearch::book::model::data to_model(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    elasticsearch::book::model::data to_model(txml::StdoutTracer tracer = txml::StdoutTracer()) const override;

    nlohmann::json to_json(txml::EmptyTracer tracer = txml::EmptyTracer()) const override;
    nlohmann::json to_json(txml::StdoutTracer tracer) const override;
private:
    void pack(const std::filesystem::path &path_to_pack) override;

    std::shared_ptr<elasticsearch::book::packer_interface_aggregator> impl;
};
}
}
#endif // ELASTIC_SEARCH_BOOK_READER_HPP
