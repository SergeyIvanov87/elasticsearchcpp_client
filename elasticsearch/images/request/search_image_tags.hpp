#ifndef ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP
#define ELASTICSEARCH_IMAGES_REQUEST_SEARCH_TAG_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/common_model/search_common_tags.hpp"

namespace elasticsearch
{
namespace image
{
namespace search
{
namespace tag
{
using namespace elasticsearch::image::search;

template<class ModelElement>
using mterm = ::model::search::must::Term<elasticsearch::image::model::data, ModelElement>;
template<class ModelElement>
using mterms = ::model::search::must::Terms<elasticsearch::image::model::data, ModelElement>;

template<class ModelElement>
using fterm = ::model::search::filter::Term<elasticsearch::image::model::data, ModelElement>;


template <class T>
inline auto make(const std::optional<T> &arg)
{
    return elasticsearch::common_model::search::tag::make<elasticsearch::image::model::data>(arg);
}
template <class T>
inline auto make(std::optional<T> &&arg)
{
    return elasticsearch::common_model::search::tag::make<elasticsearch::image::model::data>(std::move(arg));
}

inline auto make(const std::optional<elasticsearch::image::model::element::Title> &arg)
{
    return arg.has_value() ?
            std::optional<elasticsearch::v7::search::tag::simple_query_string<elasticsearch::image::model::data,
                          elasticsearch::image::model::element::Title>>(
                                elasticsearch::v7::search::tag::create::simple_query_string_tag<elasticsearch::image::model::data,
                                                                                                elasticsearch::image::model::element::Title>(arg.value().getValue())) :
            std::optional<elasticsearch::v7::search::tag::simple_query_string<elasticsearch::image::model::data,
                          elasticsearch::image::model::element::Title>>();
}

inline auto make(std::optional<elasticsearch::image::model::element::Title> &&arg)
{
    return make(arg);
}

using geo_bbox = elasticsearch::v7::search::tag::geo_bbox<elasticsearch::image::model::data,
                                                          elasticsearch::image::model::element::Location>;

inline auto make(const std::optional<geo_bbox> &arg)
{
    return arg.has_value() ?
            std::optional<geo_bbox>(arg.value()) :
            std::optional<geo_bbox>();
}

inline auto make(std::optional<geo_bbox> &&arg)
{
    return make(arg);
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
