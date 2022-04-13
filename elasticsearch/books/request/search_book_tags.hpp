#ifndef ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAG_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAG_HPP

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/v7_10/request/tags/search/tags.hpp"

namespace elasticsearch
{
namespace book
{
namespace search
{
namespace tag
{
using namespace elasticsearch::book::search;

template <class ...SubContexts>
using must = ::model::search::Must<elasticsearch::book::model::data, SubContexts...>;

template<class ModelElement>
using mterm = ::model::search::must::Term<elasticsearch::book::model::data, ModelElement>;
template<class ModelElement>
using mterms = ::model::search::must::Terms<elasticsearch::book::model::data, ModelElement>;

template<class ModelElement>
using fterm = ::model::search::filter::Term<elasticsearch::book::model::data, ModelElement>;


template <class T>
inline auto make(const std::optional<T> &arg)
{
    return arg.has_value() ? std::optional<mterm<T>>(arg.value())  : std::optional<mterm<T>>{};
}
template <class T>
inline auto make(std::optional<T> &&arg)
{
    //return elasticsearch::v7::search::tag::make<elasticsearch::v7::search::tag::Term>(std::move(arg));
    return arg.has_value() ? std::optional<mterm<T>>(std::move(arg.value())) : std::optional<mterm<T>>{};
}

inline auto make(const std::optional<elasticsearch::common_model::Tags> &arg)
{
    //return elasticsearch::v7::search::tag::make<elasticsearch::v7::search::tag::Terms>(arg);
    return arg.has_value() ? std::optional<mterms<elasticsearch::common_model::Tags>>(arg.value()) : std::optional<mterms<elasticsearch::common_model::Tags>>{};
}

inline auto make(std::optional<elasticsearch::common_model::Tags> &&arg)
{
    //return elasticsearch::v7::search::tag::make<elasticsearch::v7::search::tag::Terms>(std::move(arg));
    return arg.has_value() ? std::optional<mterms<elasticsearch::common_model::Tags>>(arg.value()) : std::optional<mterms<elasticsearch::common_model::Tags>>{};
}

template <class T, class ...Args>
inline auto make(Args &&...args)
{
    return make(std::optional<T>(std::forward<Args>(args)...));
}

namespace create
{
    template<class ...SpecificModelParams, class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Must, SpecificModelParams...>()
                              && ::model::search::details::enable_for_must_element<SpecificModelParams...>(), int>>
    must<SpecificModelParams...> must_tag(SpecificModelParams &&...args)
    {
        return must<SpecificModelParams...> (std::forward<SpecificModelParams>(args)...);
    }
    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(const std::optional<SpecificModelParams> &...args)
    {
        return must<SpecificModelParams...> (args...);
    }
} // namespace create


template<class ...SpecificQueryParams>
using query = elasticsearch::v7::search::tag::query<elasticsearch::book::model::data, SpecificQueryParams...>;
using query_all = elasticsearch::v7::search::tag::query_all;
namespace create
{
    template<class ...SpecificQueryParams>
    query<std::decay_t<SpecificQueryParams>...> query_tag(SpecificQueryParams &&...args)
    {
        return query<std::decay_t<SpecificQueryParams>...> (std::forward<SpecificQueryParams>(args)...);
    }
} // namespace create


template<class ...SpecificBooleanParams>
using boolean = elasticsearch::v7::search::tag::boolean<elasticsearch::book::model::data, SpecificBooleanParams...>;
namespace create
{
    template<class ...SpecificBooleanParams>
    boolean<std::decay_t<SpecificBooleanParams>...> boolean_tag(SpecificBooleanParams &&...args)
    {
        return boolean<std::decay_t<SpecificBooleanParams>...> (std::forward<SpecificBooleanParams>(args)...);
    }
} // namespace create

template<class ...SpecificModelElements>
using simple_query_string = elasticsearch::v7::search::tag::simple_query_string<elasticsearch::book::model::data, SpecificModelElements...>;
namespace create
{
    template<class ...SpecificModelElements>
    simple_query_string<SpecificModelElements...> simple_query_string_tag(const std::string &query_string)
    {
        return simple_query_string<SpecificModelElements...> (query_string);
    }
} // namespace create


template<class ...SortParams>
using sort  = elasticsearch::v7::search::tag::sort<elasticsearch::book::model::data, SortParams...>;
namespace create
{
    template<class ...SortParams>
    static sort<SortParams...> sort_tag(std::initializer_list<::model::Order> orders)
    {
        return sort<SortParams...> (orders);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAG_HPP
