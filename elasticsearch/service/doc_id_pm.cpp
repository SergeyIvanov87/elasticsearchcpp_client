#include "elasticsearch/service/doc_id_pm.hpp"
#include "elasticsearch/service/doc_id/model.hpp"
#include "elasticsearch/service/doc_id/schema_serializer.hpp"
#include "elasticsearch/common_model/serializers/schema_serializer.hpp"

namespace elasticsearch
{
namespace aux
{
namespace index_mapping
{
template<class Tracer>
transaction::transaction(const std::string& host, Tracer tracer) :
    impl_t(host, impl_t::Tag<elasticsearch::aux::model::data,
                             elasticsearch::aux::model::to_schema>{}, false,
                             tracer)
{
}

template transaction::transaction(const std::string& host, txml::StdoutTracer);
template transaction::transaction(const std::string& host, txml::EmptyTracer);
}// namespace index_mapping
}// namespace book
} // namespace elasticsearch
