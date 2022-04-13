#ifndef ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP
#define ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/v7_10/request/tags/search/tags.hpp"

namespace elasticsearch
{
namespace image
{
namespace search
{
namespace tag
{
using namespace elasticsearch::image::search;

template<class ...SubContexts>
using must = ::model::search::Must<elasticsearch::image::model::data, SubContexts...>;
template<class ModelElement>
using mterm = ::model::search::must::Term<elasticsearch::image::model::data, ModelElement>;
template<class ModelElement>
using mterms = ::model::search::must::Terms<elasticsearch::image::model::data, ModelElement>;

template<class ModelElement>
using fterm = ::model::search::filter::Term<elasticsearch::image::model::data, ModelElement>;


template <class T>
inline auto make(const std::optional<T> &arg)
{
    return arg.has_value() ? std::optional<mterm<T>>(arg.value())  : std::optional<mterm<T>>{};
}
template <class T>
inline auto make(std::optional<T> &&arg)
{
    return arg.has_value() ? std::optional<mterm<T>>(std::move(arg.value())) : std::optional<mterm<T>>{};
}

template <class T, class ...Args>
inline auto make(Args &&...args)
{
    return make(std::optional<T>(std::forward<Args>(args)...));
}
namespace create
{
    template<class ...SpecificModelParams>
    auto must_tag(SpecificModelParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::must_tag<elasticsearch::image::model::data>(std::forward<SpecificModelParams>(args)...);
    }
} // namespace create


template<class ...SpecificQueryParams>
using query = elasticsearch::v7::search::tag::query<elasticsearch::image::model::data, SpecificQueryParams...>;
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
using boolean = elasticsearch::v7::search::tag::boolean<elasticsearch::image::model::data, SpecificBooleanParams...>;
namespace create
{
    template<class ...SpecificBooleanParams>
    boolean<std::decay_t<SpecificBooleanParams>...> boolean_tag(SpecificBooleanParams &&...args)
    {
        return boolean<std::decay_t<SpecificBooleanParams>...> (std::forward<SpecificBooleanParams>(args)...);
    }
} // namespace create

template<class ...SpecificModelElements>
using simple_query_string = elasticsearch::v7::search::tag::simple_query_string<elasticsearch::image::model::data, SpecificModelElements...>;
namespace create
{
    template<class ...SpecificModelElements>
    simple_query_string<SpecificModelElements...> simple_query_string_tag(const std::string &query_string)
    {
        return simple_query_string<SpecificModelElements...> (query_string);
    }
}
template<class ...SortParams>
using sort  = elasticsearch::v7::search::tag::sort<elasticsearch::image::model::data, SortParams...>;
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
} // namespace image
} // namespace elasticsearch

#endif // ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP
