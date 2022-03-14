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

template<class ...SpecificModelParams>
using must = elasticsearch::v7::search::tag::must<elasticsearch::image::model::data, SpecificModelParams...>;
namespace create
{
    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(typename SpecificModelParams::value_t &&...args)
    {
        return must<SpecificModelParams...> (std::forward<typename SpecificModelParams::value_t>(args)...);
    }
    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(std::optional<typename SpecificModelParams::value_t> &&...args)
    {
        return must<SpecificModelParams...> (std::forward<std::optional<typename SpecificModelParams::value_t>>(args)...);
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
