#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP

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

template<class Model, class ...SpecificBooleanParams/*Must, Filter*/>
using boolean = ::model::search::Boolean<Model, SpecificBooleanParams...>;

namespace create
{
    template<class Model, class ...SpecificBooleanParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Boolean<Model, SpecificBooleanParams...>,
                                                                                     SpecificBooleanParams...>(), int>>
    boolean<Model, std::decay_t<SpecificBooleanParams>...> boolean_tag(SpecificBooleanParams &&...args)
    {
        static_assert(::model::search::all_of_tag<::model::search::BooleanElementTag, SpecificBooleanParams...>(),
                      "Boolean assert must be constructed from BooleanElementTag elements only");
        return boolean<Model, std::decay_t<SpecificBooleanParams>...> (std::forward<SpecificBooleanParams>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
