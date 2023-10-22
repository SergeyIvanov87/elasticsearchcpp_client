#ifndef ELASTICSEARCH_COMMON_MODEL_SEARCH_TAGS_MAPPING_HPP
#define ELASTICSEARCH_COMMON_MODEL_SEARCH_TAGS_MAPPING_HPP

#include "elasticsearch/common_model/model.hpp"
#include "elasticsearch/v7_10/request/tags/search/tags.hpp"

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
            ::elasticsearch::common_model::BinaryBlob>
{
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ::elasticsearch::common_model::BinaryBlob>;
};

template<>
struct table<model::search::RangeElementTag,
             ::elasticsearch::common_model::CreationDateTime> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::range<Model, elasticsearch::common_model::CreationDateTime>;
};

template<>
struct table<model::search::MustElementTag,
             ::elasticsearch::common_model::Description>
{
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, ::elasticsearch::common_model::Description>;
};

template<>
struct table<model::search::MustElementTag,
            ::elasticsearch::common_model::Format>
{
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ::elasticsearch::common_model::Format>;
};

template<>
struct table<model::search::MustElementTag,
            ::elasticsearch::common_model::OriginalPath>
{
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ::elasticsearch::common_model::OriginalPath>;
};

template<>
struct table<model::search::MustElementTag,
            ::elasticsearch::common_model::Preview>
{
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ::elasticsearch::common_model::Preview>;
};

template<>
struct table<model::search::MustElementTag,
            ::elasticsearch::common_model::SchemaVersion>
{
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ::elasticsearch::common_model::SchemaVersion>;
};

template<>
struct table<model::search::MustElementTag,
            ::elasticsearch::common_model::SourceName>
{
    template<class Model>
    using value_t = ::model::search::must::Term<Model, ::elasticsearch::common_model::SourceName>;
};


template<>
struct table<model::search::MustElementTag,
            ::elasticsearch::common_model::Tags>
{
    template<class Model>
    using value_t = ::model::search::must::Terms<Model, ::elasticsearch::common_model::Tags>;
};
} // namespace translation
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_COMMON_MODEL_SEARCH_TAGS_MAPPING_HPP
