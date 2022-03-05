#include "elasticsearch/images/request/image_index_mapping.hpp"
#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/images/data_model/serializers/schema_serializer.hpp"
#include "elasticsearch/common_model/serializers/schema_serializer.hpp"

namespace elasticsearch
{
namespace image
{
namespace index_mapping
{
template<class Tracer>
transaction::transaction(const std::string& host, Tracer tracer) :
    impl_t(host, impl_t::Tag<elasticsearch::image::model::data,
                             elasticsearch::image::model::to_schema,
                             elasticsearch::common_model::to_schema>{},
                             false, tracer)
{
}

template transaction::transaction(const std::string& host, txml::StdoutTracer);
template transaction::transaction(const std::string& host, txml::EmptyTracer);
}// namespace index_mapping
}// namespace image
} // namespace elasticsearch
