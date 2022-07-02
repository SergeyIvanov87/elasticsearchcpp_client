#include "elasticsearch/books/request/delete_book.hpp"

namespace elasticsearch
{
namespace book
{
namespace del
{
transaction::transaction(const std::string& host):
    impl_t(host)
{
}

transaction::~transaction() = default;

template<class Tracer>
transaction::response transaction::get_response(Tracer tracer) const
{
    return impl_t::get_response(std::move(tracer));
}

template transaction::response transaction::get_response(txml::StdoutTracer) const;
template transaction::response transaction::get_response(txml::EmptyTracer) const;
} // namespace del
} // namespace book
} // namespace elasticsearch
