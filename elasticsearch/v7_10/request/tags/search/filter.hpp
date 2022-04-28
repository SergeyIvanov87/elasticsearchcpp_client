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
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Filter<Model, SpecificModelParams...>,
                                                                                     SpecificModelParams...>()
                                      && ::model::search::all_of_tag<model::search::FilterElementTag, SpecificModelParams...>(), int>>
    filter<Model, SpecificModelParams...> filter_tag(typename SpecificModelParams::value_t &&...args)
    {
        return filter<Model, SpecificModelParams...> (std::forward<typename SpecificModelParams::value_t>(args)...);
    }

    template<class Model, class ...SpecificModelParams>
    filter<Model, SpecificModelParams...> filter_tag(const std::optional<SpecificModelParams> &...args)
    {
        static_assert(::model::search::all_of_tag<model::search::FilterElementTag, SpecificModelParams...>(),
                      "Filter assert must be constructed from FilterElementTag elements only");
        return filter<Model, SpecificModelParams...> (args...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP
