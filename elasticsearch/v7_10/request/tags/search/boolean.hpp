#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP

#include <txml/applications/json/json.hpp>

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
using boolean = ::model::search::BooleanNew<Model, SpecificBooleanParams...>;

namespace create
{
    template<class Model, class ...SpecificBooleanParams>
    boolean<Model, std::decay_t<SpecificBooleanParams>...> boolean_tag(SpecificBooleanParams &&...args)
    {
        return boolean<Model, std::decay_t<SpecificBooleanParams>...> (std::forward<SpecificBooleanParams>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
