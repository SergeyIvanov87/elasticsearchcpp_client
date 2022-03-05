#include "elasticsearch/books/request/get_book.hpp"
#include "elasticsearch/books/data_model/serializers/deserializer.hpp"
#include "elasticsearch/common_model/serializers/deserializer.hpp"

namespace elasticsearch
{
namespace book
{
namespace get
{
transaction::transaction(const std::string& host):
    impl_t(host)
{
}

transaction::~transaction() = default;

template<class Tracer>
std::shared_ptr<transaction::response> transaction::get_response(Tracer tracer) const
{
    auto response_ptr = impl_t::template get_response<response,
                                                      elasticsearch::book::model::from_data,
                                                      elasticsearch::common_model::from_data>(tracer);
    auto source_ptr = response_ptr->template getValue<::model::_Source<response>>();
    return source_ptr->template getValue<response>();
}

template std::shared_ptr<transaction::response> transaction::get_response(txml::StdoutTracer) const;
template std::shared_ptr<transaction::response> transaction::get_response(txml::EmptyTracer) const;
} // namespace get
} // namespace book
} // namespace elasticsearch
