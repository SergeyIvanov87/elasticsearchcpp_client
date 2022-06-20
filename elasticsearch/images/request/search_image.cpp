#include "elasticsearch/v7_10/answer_model/pit/object/Pit.h"
#include "elasticsearch/v7_10/request/create_pit.hpp"

#include "elasticsearch/images/request/search_image.hpp"
#include "elasticsearch/images/data_model/serializers/deserializer.hpp"
#include "elasticsearch/images/data_model/serializers/serializer.hpp"
#include "elasticsearch/common_model/serializers/deserializer.hpp"
#include "elasticsearch/common_model/serializers/serializer.hpp"


namespace elasticsearch
{
namespace image
{
namespace search
{
const transaction::receiver& transaction::get_receiver() const
{
    return base_t::get_receiver();
}

template<class Tracer>
std::optional<transaction::response> transaction::get_response(Tracer tracer) const
{
    auto response_ptr = base_t::template get_response<model_t,
                                                      elasticsearch::image::model::from_data,
                                                      elasticsearch::common_model::from_data>(tracer);
    return response_ptr;
}
template std::optional<transaction::response> transaction::get_response(txml::StdoutTracer) const;
template std::optional<transaction::response> transaction::get_response(txml::EmptyTracer) const;
}// namespace search
}// namespace image
} // namespace elasticsearch
