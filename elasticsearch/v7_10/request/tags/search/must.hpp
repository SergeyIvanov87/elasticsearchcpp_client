#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP

#include <optional>

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

template<class Model, class ...SpecificModelParams>
using must = ::model::search::Must<Model, SpecificModelParams...>;

namespace create
{
    template<class Model, class ...SpecificModelParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Must, SpecificModelParams...>()
                                      && ::model::search::details::enable_for_must_element<SpecificModelParams...>(), int>>
    must<Model, SpecificModelParams...> must_tag(SpecificModelParams &&...args)
    {
        return must<Model, SpecificModelParams...> (std::forward<SpecificModelParams>(args)...);
    }

    template<class Model, class ...SpecificModelParams>
    must<Model, SpecificModelParams...> must_tag(const std::optional<SpecificModelParams> &...args)
    {
        return must<Model, SpecificModelParams...> (args...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
