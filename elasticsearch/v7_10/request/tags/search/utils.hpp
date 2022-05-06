#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_UTILS_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_UTILS_HPP

#include <optional>

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;
namespace translation
{
template<class SearchTag, class ModelElement>
struct table {
    template<class Model>
    using value_t = ModelElement;
};

template<class Model, class SearchTag>
struct table_mapper
{
    template<class ModelElement>
    static auto map(const ModelElement &v)
    {
        return typename table<SearchTag, ModelElement>::value_t<Model>(v);
    }

    template<class ModelElement>
    static auto map(const std::optional<ModelElement> &v)
    {
        return v.has_value() ?
                    std::optional<decltype(map<ModelElement>(std::declval<ModelElement>()))>(v.value()) :
                    std::optional<decltype(map<ModelElement>(std::declval<ModelElement>()))>();
    }
};
} // namespace translation

template<class T>
struct decay_optional { using type = T; };
template<class T>
struct decay_optional<std::optional<T>> { using type = T; };

template<class T>
using decay_optional_t = typename decay_optional<T>::type;
} // namespace tag
} // namespace elasticsearch
} // namespace v7
} // namespace search

#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_UTILS_HPP
