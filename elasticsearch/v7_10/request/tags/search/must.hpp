#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP

#include <optional>

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/request/tags/search/utils.hpp"
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

namespace translation
{
template<class ModelElement>
struct table<model::search::MustElementTag,
             ModelElement> {
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ModelElement>;
};
}

namespace create
{
    template<class Model, class ...SpecificModelParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Must<Model,
                                                                                                           elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>,
                                                                                     elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>()
                                      && ::model::search::all_of_tag<model::search::MustElementTag,
                                                                     elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>(), int>>
    must<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>
    must_tag(SpecificModelParams &&...args)
    {
        return must<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...> (
            elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::MustElementTag>::template map(std::forward<SpecificModelParams>(args))...);
    }

    template<class Model, class ...SpecificModelParams>
    std::optional<must<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>>
    must_optional_tag(SpecificModelParams &&...args)
    {
        static_assert(::model::search::all_of_tag<model::search::MustElementTag,
                      elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>(),
                      "Must assert must be constructed from MustElementTag elements only");
        return txml::GenericCreator::try_create<must<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::MustElementTag, SpecificModelParams>...>> (
            elasticsearch::v7::search::tag::translation::table_mapper<Model, model::search::MustElementTag>::template map(std::forward<SpecificModelParams>(args))...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
