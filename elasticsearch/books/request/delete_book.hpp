#ifndef ELASTICSEARCH_BOOKS_REQUEST_DELETE_BOOK_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_DELETE_BOOK_HPP

#include "elasticsearch/v7_10/request/delete_data.hpp"
namespace elasticsearch
{
namespace book
{
namespace del
{
class transaction : public elasticsearch::v7::delete_data::transaction
{
public:
    using impl_t = elasticsearch::v7::delete_data::transaction;
    using response = elasticsearch::v7::delete_data::response;

    static constexpr const char *name() { return "book/del"; }

    transaction(const std::string& host);
    ~transaction();

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer = Tracer()) const;
};
} // namespace del
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_REQUEST_DELETE_BOOK_HPP
