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

template<class ...SpecificModelParams>
using must = elasticsearch::v7::search::tag::must<elasticsearch::book::model::data, SpecificModelParams...>;
namespace create
{
namespace details
{
template <class T, class V>
std::optional<elasticsearch::v7::search::tag::details::CArg<T&&, elasticsearch::v7::search::tag::Term>> make_tag(const std::optional<V> &arg)
{
    using t_t = elasticsearch::v7::search::tag::Term;
        return arg.has_value() ?  elasticsearch::v7::search::tag::make<t_t>(T{V{arg.value()}}) :
                std::optional<elasticsearch::v7::search::tag::details::CArg<T&&, t_t>>{};
}
template <class T>
inline std::optional<elasticsearch::v7::search::tag::details::CArg<const T&, elasticsearch::v7::search::tag::Term>> make(const std::optional<T> &arg)
{
    using t_t = elasticsearch::v7::search::tag::Term;
        return arg.has_value() ?  elasticsearch::v7::search::tag::make<t_t>(arg.value()) :
                std::optional<elasticsearch::v7::search::tag::details::CArg<const T&, t_t>>{};
}


inline std::optional<elasticsearch::v7::search::tag::details::CArg<const elasticsearch::common_model::Tags&, elasticsearch::v7::search::tag::Terms>> make(const std::optional<elasticsearch::common_model::Tags> &arg)
{
    using t_t = elasticsearch::v7::search::tag::Terms;
        return arg.has_value() ?  elasticsearch::v7::search::tag::make<t_t>(arg.value()) :
                std::optional<elasticsearch::v7::search::tag::details::CArg<const elasticsearch::common_model::Tags&, t_t>>{};
}
}
    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(typename SpecificModelParams::value_t &&...args)
    {
        return must<SpecificModelParams...> (std::forward<typename SpecificModelParams::value_t>(args)...);
    }

    template<class ...SpecificModelParams>
    must<typename SpecificModelParams::value_t...> must_tag_ext(const std::optional<SpecificModelParams> &...args);

    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(const std::optional<typename SpecificModelParams::value_t>&...args)
    {
        return must_tag_ext(details::make_tag<SpecificModelParams>(args)...);
    }

    template<class ...SpecificModelParams>
    must<typename SpecificModelParams::value_t...> must_tag_ext(SpecificModelParams &&...args)
    {
        return must<typename SpecificModelParams::value_t...> (std::forward<SpecificModelParams>(args)...);
    }

    template<class ...SpecificModelParams>
    must<typename SpecificModelParams::value_t...> must_tag_ext(const std::optional<SpecificModelParams> &...args)
    {
        return must<typename SpecificModelParams::value_t...> (args...);
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
