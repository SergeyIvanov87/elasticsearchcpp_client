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
    template<class Model, class ...SpecificBooleanParams>
    std::optional<boolean<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::BooleanElementTag, SpecificBooleanParams>...>>
    boolean_tag(SpecificBooleanParams &&...args)
    {
        static_assert(::model::search::all_of_tag<::model::search::BooleanElementTag,
                       elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, ::model::search::BooleanElementTag, SpecificBooleanParams>...>(),
                      "Boolean assert must be constructed from BooleanElementTag elements only");
        return txml::GenericCreator::try_create<boolean<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::BooleanElementTag, SpecificBooleanParams>...>> (
            elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::BooleanElementTag>::template map(std::forward<SpecificBooleanParams>(args))...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
