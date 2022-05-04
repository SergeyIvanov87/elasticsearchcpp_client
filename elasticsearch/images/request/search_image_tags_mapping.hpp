#ifndef ELASTICSEARCH_IMAGE_REQUEST_SEARCH_TAGS_MAPPING_HPP
#define ELASTICSEARCH_IMAGE_REQUEST_SEARCH_TAGS_MAPPING_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/common_model/search_common_tags_mapping.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
namespace must_helper
{
namespace translation
{
template<>
struct table<elasticsearch::image::model::element::Title {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, elasticsearch::image::model::element::Title>;
};
} // namespace translation
} // namespace must_helper

} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch

#endif // ELASTICSEARCH_IMAGE_REQUEST_SEARCH_TAGS_MAPPING_HPP
