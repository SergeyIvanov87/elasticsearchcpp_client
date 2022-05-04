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

template<class Model>
struct table_mapper
{
    template<class ModelElement>
    static auto map(const ModelElement &v)
    {
        return typename table<ModelElement>::value_t<Model>(v);
    }

    template<class ModelElement>
    static auto map(const std::optional<ModelElement> &v)
    {
        return v.has_value() ?
                    std::optional<decltype(map<ModelElement>(std::declval<ModelElement>()))>(v.value()) :
                    std::optional<decltype(map<ModelElement>(std::declval<ModelElement>()))>();
    }
};

template<class T>
struct decay_optional { using type = T; };
template<class T>
struct decay_optional<std::optional<T>> { using type = T; };
template<class T>
using decay_optional_t = typename decay_optional<T>::type;
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
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Must<Model,
                                                                                                           elasticsearch::v7::search::tag::must_helper::translation::decay_optional_t<decltype(elasticsearch::v7::search::tag::must_helper::translation::table_mapper<Model>::template map(std::declval<SpecificModelParams>()))>...>,
                                                                                     elasticsearch::v7::search::tag::must_helper::translation::decay_optional_t<decltype(elasticsearch::v7::search::tag::must_helper::translation::table_mapper<Model>::template map(std::declval<SpecificModelParams>()))>...>()
                                      && ::model::search::all_of_tag<model::search::MustElementTag,
                                                                     elasticsearch::v7::search::tag::must_helper::translation::decay_optional_t<decltype(elasticsearch::v7::search::tag::must_helper::translation::table_mapper<Model>::template map(std::declval<SpecificModelParams>()))>...>(), int>>
    must<Model, elasticsearch::v7::search::tag::must_helper::translation::decay_optional_t<decltype(elasticsearch::v7::search::tag::must_helper::translation::table_mapper<Model>::template map(std::declval<SpecificModelParams>()))>...>
    must_tag(SpecificModelParams &&...args)
    {
        return must<Model, elasticsearch::v7::search::tag::must_helper::translation::decay_optional_t<decltype(elasticsearch::v7::search::tag::must_helper::translation::table_mapper<Model>::template map(std::declval<SpecificModelParams>()))>...> (
            elasticsearch::v7::search::tag::must_helper::translation::table_mapper<Model>::template map(std::forward<SpecificModelParams>(args))...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
