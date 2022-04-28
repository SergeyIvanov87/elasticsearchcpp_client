#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_GEO_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_GEO_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/object/geo/GeoBoundingBox.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;

template<class Model, class ModelElement>
using geo_bbox = ::model::search::GeoBoundingBox<Model, ModelElement>;

namespace create
{
    template<class Model, class ModelElement, class ...Args,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::GeoBoundingBox<Model, ModelElement>,
                                                                                     Args...>(), int>>
    geo_bbox<Model, ModelElement> geo_bbox_tag(Args &&...args)
    {
        static_assert(::model::search::all_of_tag<::model::search::GeoElementTag, Args...>(),
                      "GeoBoundingBox assert must be constructed from GeoElementTag elements only");
        return geo_bbox<Model, ModelElement> (std::forward<Args>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_GEO_HPP
