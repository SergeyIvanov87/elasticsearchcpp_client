#ifndef ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAGS_MAPPING_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAGS_MAPPING_HPP

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/common_model/search_common_tags_mapping.hpp"

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
             elasticsearch::book::model::element::Title>
{
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, elasticsearch::book::model::element::Title>;
};

template<>
struct table<model::search::MustElementTag,
             elasticsearch::book::model::element::Contributor>
{
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, elasticsearch::book::model::element::Contributor>;
};

template<>
struct table<model::search::MustElementTag,
             elasticsearch::book::model::element::Creator>
{
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, elasticsearch::book::model::element::Creator>;
};
} // namespace translation

} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch

#endif // ELASTICSEARCH_BOOKS_REQUEST_SEARCH_TAGS_MAPPING_HPP
