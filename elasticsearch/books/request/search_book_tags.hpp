#ifndef ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAG_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAG_HPP

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/common_model/search_common_tags.hpp"
#include "elasticsearch/books/request/search_book_tags_mapping.hpp"

namespace elasticsearch
{
namespace book
{
namespace search
{
namespace tag
{
using namespace elasticsearch::book::search;
namespace create
{
    template<class ...SpecificModelParams>
    auto must_tag(SpecificModelParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::must_optional_tag<elasticsearch::book::model::data>(std::forward<SpecificModelParams>(args)...);
    }

    template<class ...RangedElements>
    auto range_tag(const std::array<std::string, sizeof...(RangedElements)> &ranged_string, char sep = ',')
    {
        return elasticsearch::v7::search::tag::range<elasticsearch::book::model::data, RangedElements...>(ranged_string, sep);
    }

    template<class ...RangedElements>
    auto range_tag(const std::array<std::optional<std::string>, sizeof...(RangedElements)> &ranged_string, char sep = ',')
    {
        return elasticsearch::v7::search::tag::create::range_tag<elasticsearch::book::model::data, RangedElements...>(ranged_string, sep);
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
        return elasticsearch::v7::search::tag::create::query_optional_tag<elasticsearch::book::model::data>(std::forward<SpecificQueryParams>(args)...);
    }

    template<class ...SpecificBooleanParams>
    auto boolean_tag(SpecificBooleanParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::boolean_optional_tag<elasticsearch::book::model::data>(std::forward<SpecificBooleanParams>(args)...);
    }

    template<class ...SpecificModelElements>
    auto simple_query_string_tag(const std::optional<std::string> &query_string)
    {
        return elasticsearch::v7::search::tag::create::simple_query_string_optional_tag<elasticsearch::book::model::data,
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
