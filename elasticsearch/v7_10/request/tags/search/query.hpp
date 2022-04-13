#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/serializer.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/new_Bool.h"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;
template<class Model, class ...SpecificQueryParams>
using query = model::search::QueryNew<Model, SpecificQueryParams...>;
using query_all = query<::model::EmptyModel, ::model::EmptyParam>;
namespace create
{
    template<class Model, class ...SpecificQueryParams>
    query<Model, std::decay_t<SpecificQueryParams>...> query_tag(SpecificQueryParams &&...args)
    {
        return query<Model, std::decay_t<SpecificQueryParams>...> (std::forward<SpecificQueryParams>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP
