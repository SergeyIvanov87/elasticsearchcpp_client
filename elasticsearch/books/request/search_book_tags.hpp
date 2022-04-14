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

template<class ModelElement>
using mterm = ::model::search::must::Term<elasticsearch::book::model::data, ModelElement>;
template<class ModelElement>
using mterms = ::model::search::must::Terms<elasticsearch::book::model::data, ModelElement>;

template<class ModelElement>
using fterm = ::model::search::filter::Term<elasticsearch::book::model::data, ModelElement>;


template <class T>
inline auto make(const std::optional<T> &arg)
{
    return elasticsearch::v7::search::tag::make_term<elasticsearch::book::model::data>(arg);
}
template <class T>
inline auto make(std::optional<T> &&arg)
{
    return elasticsearch::v7::search::tag::make_term<elasticsearch::book::model::data>(std::move(arg));
}

inline auto make(const std::optional<elasticsearch::common_model::Tags> &arg)
{
    return elasticsearch::v7::search::tag::make_terms<elasticsearch::book::model::data>(arg);
}

inline auto make(std::optional<elasticsearch::common_model::Tags> &&arg)
{
    return elasticsearch::v7::search::tag::make_terms<elasticsearch::book::model::data>(std::move(arg));
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
        return elasticsearch::v7::search::tag::create::must_tag<elasticsearch::book::model::data>(std::forward<SpecificModelParams>(args)...);
    }
} // namespace create


template<class ...SpecificQueryParams>
using query = elasticsearch::v7::search::tag::query<elasticsearch::book::model::data, SpecificQueryParams...>;
using query_all = elasticsearch::v7::search::tag::query_all;
namespace create
{
    template<class ...SpecificQueryParams>
    auto query_tag(SpecificQueryParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::query_tag<elasticsearch::book::model::data>(std::forward<SpecificQueryParams>(args)...);
    }

    template<class ...SpecificBooleanParams>
    auto boolean_tag(SpecificBooleanParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::boolean_tag<elasticsearch::book::model::data>(std::forward<SpecificBooleanParams>(args)...);
    }

    template<class ...SpecificModelElements>
    auto simple_query_string_tag(const std::string &query_string)
    {
        return elasticsearch::v7::search::tag::create::simple_query_string_tag<elasticsearch::book::model::data,
                                                                               SpecificModelElements...>(query_string);
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
