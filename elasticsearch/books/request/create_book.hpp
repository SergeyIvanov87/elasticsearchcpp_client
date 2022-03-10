#ifndef ELASTICSEARCH_BOOKS_REQUEST_CREATE_BOOK_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_CREATE_BOOK_HPP

#include "elasticsearch/v7_10/request/put_data.hpp"
#include "elasticsearch/books/data_model/model.hpp"

namespace elasticsearch
{
namespace book
{
namespace create
{
class transaction
{
public:
    using impl_t = elasticsearch::v7::put_json_data::transaction;
    using receiver = impl_t::receiver;
    using response = elasticsearch::v7::put_json_data::response;

    static constexpr const char *name() { return "book/create"; }

    transaction(const std::string& host);
    ~transaction();

    template<class Tracer = txml::EmptyTracer>
    void execute_force_refresh(const std::string& index_name, const elasticsearch::book::model::data &instance,
                               bool curl_verbose = false, Tracer tracer = Tracer());

    template<class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, const elasticsearch::book::model::data &instance,
                 bool curl_verbose = false, Tracer tracer = Tracer());

    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<response> get_response(Tracer tracer = Tracer()) const;
private:
    std::unique_ptr<impl_t> generic_transaction_ptr;
};
} // namespace create
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_REQUEST_CREATE_BOOK_HPP
