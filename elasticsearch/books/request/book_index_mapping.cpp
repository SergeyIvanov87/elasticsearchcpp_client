#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/data_model/serializers/schema_serializer.hpp"
#include "elasticsearch/common_model/serializers/schema_serializer.hpp"

namespace elasticsearch
{
namespace book
{
namespace index_mapping
{
template<class Tracer>
transaction::transaction(const std::string& host, Tracer tracer) :
    impl_t(host, impl_t::Tag<elasticsearch::book::model::data,
                             elasticsearch::book::model::to_schema,
                             elasticsearch::common_model::to_schema>{}, false,
                             tracer)
{
}

template transaction::transaction(const std::string& host, txml::StdoutTracer);
template transaction::transaction(const std::string& host, txml::EmptyTracer);
}// namespace index_mapping
}// namespace book
} // namespace elasticsearch
