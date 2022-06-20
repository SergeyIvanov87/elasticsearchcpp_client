#include "elasticsearch/books/request/create_book.hpp"
#include "elasticsearch/books/data_model/serializers/serializer.hpp"
#include "elasticsearch/common_model/serializers/serializer.hpp"

namespace elasticsearch
{
namespace book
{
namespace create
{
transaction::transaction(const std::string& host)
{
    generic_transaction_ptr.reset(new impl_t(host, false));
}

transaction::~transaction() = default;

template<class Tracer>
void transaction::execute_force_refresh(const std::string& index_name,
                                        const elasticsearch::book::model::data &instance,
                                        bool curl_verbose/* = false*/,
                                        Tracer tracer/* = Tracer()*/)
{
    generic_transaction_ptr->execute_force_refresh(index_name,
                                                   impl_t::Tag<elasticsearch::book::model::data,
                                                               elasticsearch::book::to_data,
                                                               elasticsearch::common_model::to_data>{instance},
                                                   curl_verbose,
                                                   tracer);
}

template<class Tracer>
void transaction::execute(const std::string& index_name,
                          const elasticsearch::book::model::data &instance,
                          bool curl_verbose/* = false*/,
                          Tracer tracer/* = Tracer()*/)
{
    generic_transaction_ptr->execute(index_name,
                                     impl_t::Tag<elasticsearch::book::model::data,
                                                 elasticsearch::book::to_data,
                                                 elasticsearch::common_model::to_data>{instance},
                                     curl_verbose,
                                     tracer);
}

const transaction::receiver& transaction::get_receiver() const
{
    return generic_transaction_ptr->get_receiver();
}

template<class Tracer>
std::optional<transaction::response> transaction::get_response(Tracer tracer) const
{
    return generic_transaction_ptr->get_response(tracer);
}

template void transaction::execute_force_refresh(const std::string& index_name,
                                                 const elasticsearch::book::model::data &instance,
                                                 bool curl_verbose,
                                                 txml::StdoutTracer);
template void transaction::execute_force_refresh(const std::string& index_name,
                                                 const elasticsearch::book::model::data &instance,
                                                 bool curl_verbose,
                                                 txml::EmptyTracer);

template void transaction::execute(const std::string& index_name,
                                   const elasticsearch::book::model::data &instance,
                                   bool curl_verbose,
                                   txml::StdoutTracer);
template void transaction::execute(const std::string& index_name,
                                   const elasticsearch::book::model::data &instance,
                                   bool curl_verbose,
                                   txml::EmptyTracer);

template std::optional<transaction::response> transaction::get_response(txml::StdoutTracer) const;
template std::optional<transaction::response> transaction::get_response(txml::EmptyTracer) const;
}// namespace create
}// namespace book
} // namespace elasticsearch
