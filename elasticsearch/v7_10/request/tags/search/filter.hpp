#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/object/boolean/Bool.h"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;
// the same as must
template<class Model, class ...SpecificModelParams>
using filter = ::model::search::Filter<Model, SpecificModelParams...>;

namespace create
{
    template<class Model, class ...SpecificModelParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Filter<Model,
                                                                                                           elasticsearch::v7::search::tag::decay_optional_t<decltype(elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::FilterElementTag>::template map(std::declval<SpecificModelParams>()))>...>,
                                                                                     elasticsearch::v7::search::tag::decay_optional_t<decltype(elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::FilterElementTag>::template map(std::declval<SpecificModelParams>()))>...>()
                                      && ::model::search::all_of_tag<model::search::FilterElementTag,
                                                                     elasticsearch::v7::search::tag::decay_optional_t<decltype(elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::FilterElementTag>::template map(std::declval<SpecificModelParams>()))>...>(), int>>
    filter<Model, elasticsearch::v7::search::tag::decay_optional_t<decltype(elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::FilterElementTag>::template map(std::declval<SpecificModelParams>()))>...>
    filter_tag(SpecificModelParams &&...args)
    {
        return filter<Model, elasticsearch::v7::search::tag::decay_optional_t<decltype(elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::FilterElementTag>::template map(std::declval<SpecificModelParams>()))>...> (
            elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::FilterElementTag>::template map(std::forward<SpecificModelParams>(args))...);
    }

} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP
