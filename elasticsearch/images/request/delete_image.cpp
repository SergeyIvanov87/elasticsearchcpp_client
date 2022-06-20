#include "elasticsearch/images/request/delete_image.hpp"

namespace elasticsearch
{
namespace image
{
namespace del
{
transaction::transaction(const std::string& host):
    impl_t(host)
{
}

transaction::~transaction() = default;

template<class Tracer>
std::optional<transaction::response> transaction::get_response(Tracer tracer) const
{
    return impl_t::get_response(std::move(tracer));
}

template std::optional<transaction::response> transaction::get_response(txml::StdoutTracer) const;
template std::optional<transaction::response> transaction::get_response(txml::EmptyTracer) const;
} // namespace del
} // namespace image
} // namespace elasticsearch
