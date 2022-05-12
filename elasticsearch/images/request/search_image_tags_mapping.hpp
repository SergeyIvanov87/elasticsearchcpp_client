#ifndef ELASTICSEARCH_IMAGE_REQUEST_SEARCH_TAGS_MAPPING_HPP
#define ELASTICSEARCH_IMAGE_REQUEST_SEARCH_TAGS_MAPPING_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/common_model/search_common_tags_mapping.hpp"
#include "elasticsearch/images/request/search_image_tags.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
namespace translation
{
template<>
struct table<model::search::MustElementTag,
             elasticsearch::image::model::element::Title> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, elasticsearch::image::model::element::Title>;
};

template<>
struct table<model::search::FilterElementTag,
              elasticsearch::v7::search::tag::geo_bbox<elasticsearch::image::model::data,
                                                       elasticsearch::image::model::element::Location>> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::geo_bbox<elasticsearch::image::model::data,
                                                             elasticsearch::image::model::element::Location>;
};

template<>
struct table<model::search::RangeElementTag,
             elasticsearch::image::model::element::OriginalTime> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::range<Model, elasticsearch::image::model::element::OriginalTime>;
};

template<>
struct table<model::search::RangeElementTag,
             elasticsearch::image::model::element::DigitizeTime> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::range<Model, elasticsearch::image::model::element::DigitizeTime>;
};
} // namespace translation

} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch

#endif // ELASTICSEARCH_IMAGE_REQUEST_SEARCH_TAGS_MAPPING_HPP
