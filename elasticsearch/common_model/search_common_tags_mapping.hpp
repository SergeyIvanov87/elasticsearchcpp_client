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
namespace must_helper
{
namespace translation
{
template<>
struct table<::elasticsearch::common_model::Tags> {
    template<class Model>
    using value_t = ::model::search::must::Terms<Model, ::elasticsearch::common_model::Tags>;
};

template<>
struct table<::elasticsearch::common_model::Description> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, ::elasticsearch::common_model::Description>;
};
}
}
}
}
}
} // namespace elasticsearch
#endif // ELASTICSEARCH_COMMON_MODEL_SEARCH_TAGS_MAPPING_HPP