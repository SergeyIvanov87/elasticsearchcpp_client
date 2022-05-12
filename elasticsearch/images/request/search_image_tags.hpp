#ifndef ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP
#define ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/common_model/search_common_tags.hpp"
#include "elasticsearch/images/request/search_image_tags_mapping.hpp"

namespace elasticsearch
{
namespace image
{
namespace search
{
namespace tag
{
using namespace elasticsearch::image::search;

using geo_bbox = elasticsearch::v7::search::tag::geo_bbox<elasticsearch::image::model::data,
                                                          elasticsearch::image::model::element::Location>;

namespace create
{
    template<class ...SpecificModelParams>
    auto must_tag(SpecificModelParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::must_tag<elasticsearch::image::model::data>(std::forward<SpecificModelParams>(args)...);
    }
} // namespace create

namespace create
{
    template<class ...SpecificModelParams>
    auto filter_tag(SpecificModelParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::filter_tag<elasticsearch::image::model::data>(std::forward<SpecificModelParams>(args)...);
    }
} // namespace create


template<class ...SpecificQueryParams>
using query = elasticsearch::v7::search::tag::query<elasticsearch::image::model::data, SpecificQueryParams...>;
using query_all = elasticsearch::v7::search::tag::query_all;
namespace create
{
    template<class ...SpecificQueryParams>
    auto query_tag(SpecificQueryParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::query_tag<elasticsearch::image::model::data>(std::forward<SpecificQueryParams>(args)...);
    }

    template<class ...SpecificBooleanParams>
    auto boolean_tag(SpecificBooleanParams &&...args)
    {
        return elasticsearch::v7::search::tag::create::boolean_tag<elasticsearch::image::model::data>(std::forward<SpecificBooleanParams>(args)...);
    }

    template<class ...SpecificModelElements>
    auto simple_query_string_tag(const std::string &query_string)
    {
        return elasticsearch::v7::search::tag::create::simple_query_string_tag<elasticsearch::image::model::data,
                                                                               SpecificModelElements...>(query_string);
    }
} // namespace create

namespace create
{
    template<class ...RangedElements>
    auto range_tag(const std::array<std::string, sizeof...(RangedElements)> &ranged_string, char sep = ',')
    {
        return elasticsearch::v7::search::tag::range_element<elasticsearch::image::model::data, RangedElements...>(ranged_string, sep);
    }

    template<class ...RangedElements>
    auto range_tag(const std::array<std::optional<std::string>, sizeof...(RangedElements)> &ranged_string, char sep = ',')
    {
        return elasticsearch::v7::search::tag::range_element<elasticsearch::image::model::data, RangedElements...>(ranged_string, sep);
    }

    template<class RangedElement, template<class> class Limit, class Value>
    auto range_tag(const Value &l)
    {
        return elasticsearch::v7::search::tag::create::range_tag<elasticsearch::image::model::data, RangedElement>(Limit<Value>(l));
    }

    template<class RangedElement, class Value_1, class Value_2>
    auto range_tag(const Value_1 &l1, const Value_2 &l2)
    {
        return elasticsearch::v7::search::tag::create::range_tag<elasticsearch::image::model::data, RangedElement>(l1, l2);
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
