#ifndef ELASTICSEARCH_BOOKS_REQUEST_INDEX_MAPPING_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_INDEX_MAPPING_HPP

#include "elasticsearch/v7_10/request/index_mapping.hpp"

namespace elasticsearch
{
namespace book
{
namespace index_mapping
{
class transaction : public elasticsearch::v7::index_mapping::transaction
{
public:
    using impl_t = elasticsearch::v7::index_mapping::transaction;
    using impl_t::receiver;
    using impl_t::response;

    template<class Tracer = txml::EmptyTracer>
    transaction(const std::string& host, Tracer tracer = Tracer());
};
} // namespace index_mapping
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_HPP
