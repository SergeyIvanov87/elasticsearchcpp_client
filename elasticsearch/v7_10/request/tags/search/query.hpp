#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/serializer.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/Bool.h"
#include "elasticsearch/v7_10/answer_model/search/object/Query.h"

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
using query = model::search::Query<Model, SpecificQueryParams...>;
using query_all = query<::model::EmptyModel, ::model::EmptyParam>;
namespace create
{
    template<class Model, class ...SpecificQueryParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Query<Model,
                                                                                                            elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>,
                                                                                     elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>()
                                      && ::model::search::all_of_tag<model::search::QueryElementTag,
                                                                     elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>(), int>>
    query<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...> query_tag(SpecificQueryParams &&...args)
    {
        static_assert(::model::search::all_of_tag<model::search::QueryElementTag,
                      elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>(),
                      "Query assert must be constructed from QueryElementTag elements only");
        //return query<Model, std::decay_t<SpecificQueryParams>...> (std::forward<SpecificQueryParams>(args)...);
        return query<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...> (
            elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::QueryElementTag>::template map(std::forward<SpecificQueryParams>(args))...);
    }

    template<class Model, class ...SpecificQueryParams>
    std::optional<query<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>>
    query_optional_tag(SpecificQueryParams &&...args)
    {
        static_assert(::model::search::all_of_tag<model::search::QueryElementTag,
                      elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>(),
                      "Query assert must be constructed from QueryElementTag elements only");
        return txml::GenericCreator::try_create<query<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::QueryElementTag, SpecificQueryParams>...>> (
            elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::QueryElementTag>::template map(std::forward<SpecificQueryParams>(args))...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP
