#include "elasticsearch/images/request/get_image.hpp"
#include "elasticsearch/images/data_model/serializers/deserializer.hpp"
#include "elasticsearch/common_model/serializers/deserializer.hpp"

namespace elasticsearch
{
namespace image
{
namespace get
{
transaction::transaction(const std::string& host):
    impl_t(host)
{
}

transaction::~transaction() = default;

template<class Tracer>
std::optional<transaction::response> transaction::get_response(Tracer tracer) const
{
    auto response_ptr = impl_t::template get_response<response,
                                                      elasticsearch::image::model::from_data,
                                                      elasticsearch::common_model::from_data>(tracer);
    const auto &source = response_ptr->template value<::model::_Source<response>>();
    return source.template node<response>();
}

template std::optional<transaction::response> transaction::get_response(txml::StdoutTracer) const;
template std::optional<transaction::response> transaction::get_response(txml::EmptyTracer) const;
} // namespace get
} // namespace image
} // namespace elasticsearch
