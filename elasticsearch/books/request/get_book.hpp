#ifndef ELASTICSEARCH_BOOKS_REQUEST_GET_BOOK_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_GET_BOOK_HPP

#include "elasticsearch/v7_10/request/get_data.hpp"
#include "elasticsearch/books/data_model/model.hpp"

// forward declarations
class Connection;

namespace elasticsearch
{
namespace book
{
namespace get
{
class transaction : public elasticsearch::v7::get_json_data::transaction
{
public:
    using impl_t = elasticsearch::v7::get_json_data::transaction;
    using receiver = impl_t::receiver;
    using response = elasticsearch::book::model::data;

    static constexpr const char *name() { return "book/get"; }

    transaction(const std::string& host);
    ~transaction();

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer = Tracer()) const;
};
} // namespace get
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_REQUEST_GET_BOOK_HPP
