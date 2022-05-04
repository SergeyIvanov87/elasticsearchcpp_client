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

namespace must_helper
{
namespace translation
{
template<class ModelElement>
struct table {
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ModelElement>;
};
}
}

template <class Model, class ModelElement>
inline auto make_term(const std::optional<ModelElement> &arg)
{
    return arg.has_value() ?
           std::optional<::model::search::must::Term<Model, ModelElement>>(arg.value()) :
           std::optional<::model::search::must::Term<Model, ModelElement>>{};
}

template <class Model, class ModelElement>
inline auto make_term(std::optional<ModelElement> &&arg)
{
    return arg.has_value() ?
           std::optional<::model::search::must::Term<Model, ModelElement>>(std::move(arg.value())) :
           std::optional<::model::search::must::Term<Model, ModelElement>>{};
}

template <class Model, class ModelElement>
inline auto make_terms(const std::optional<ModelElement> &arg)
{
    return arg.has_value() ?
           std::optional<::model::search::must::Terms<Model, ModelElement>>(arg.value()) :
           std::optional<::model::search::must::Terms<Model, ModelElement>>{};
}

template <class Model, class ModelElement>
inline auto make_terms(std::optional<ModelElement> &&arg)
{
    return arg.has_value() ?
           std::optional<::model::search::must::Terms<Model, ModelElement>>(std::move(arg.value())) :
           std::optional<::model::search::must::Terms<Model, ModelElement>>{};
}

namespace create
{
    template<class Model, class ...SpecificModelParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Must<Model, SpecificModelParams...>,
                                                                                     SpecificModelParams...>()
                                      && ::model::search::all_of_tag<model::search::MustElementTag, SpecificModelParams...>(), int>>
    must<Model, SpecificModelParams...> must_tag(SpecificModelParams &&...args)
    {
        return must<Model, SpecificModelParams...> (std::forward<SpecificModelParams>(args)...);
    }

    template<class Model, class ...SpecificModelParams,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Must<Model,
                                                                                                           typename elasticsearch::v7::search::tag::must_helper::translation::table<SpecificModelParams>::value_t<Model>...>,
                                                                                     typename elasticsearch::v7::search::tag::must_helper::translation::table<SpecificModelParams>::value_t<Model>...>()
                                      && ::model::search::all_of_tag<model::search::MustElementTag, typename elasticsearch::v7::search::tag::must_helper::translation::table<SpecificModelParams>::value_t<Model>...>(), int>>
    must<Model, typename elasticsearch::v7::search::tag::must_helper::translation::table<SpecificModelParams>::value_t<Model>...> must_raw_tag(SpecificModelParams &&...args)
    {
        return must<Model, typename elasticsearch::v7::search::tag::must_helper::translation::table<SpecificModelParams>::value_t<Model>...> (
            typename elasticsearch::v7::search::tag::must_helper::translation::table<SpecificModelParams>::value_t<Model>(std::forward<SpecificModelParams>(args))...);
    }

    template<class Model, class ...SpecificModelParams>
    must<Model, SpecificModelParams...> must_tag(const std::optional<SpecificModelParams> &...args)
    {
        static_assert(::model::search::all_of_tag<model::search::MustElementTag, SpecificModelParams...>(),
                      "Must assert must be constructed from MustElementTag elements only");
        return must<Model, SpecificModelParams...> (args...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
