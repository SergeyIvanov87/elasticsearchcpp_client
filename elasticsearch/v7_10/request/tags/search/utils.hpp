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

template<class M, class E>
struct NonMappedType
{
private:
    NonMappedType(...) = delete;
    NonMappedType() = delete;
};

template<class Model, class SearchTag>
struct table_mapper
{
    template<class ModelElement, class = std::enable_if_t<!std::is_same_v<typename table<SearchTag, ModelElement>::template value_t<Model>, NonMappedType<Model, ModelElement>>, int>>
    static auto map(const ModelElement &v)
    {
        return typename table<SearchTag, ModelElement>::template value_t<Model>(v);
    }

    template<class ModelElement, std::enable_if_t<std::is_same_v<typename table<SearchTag, ModelElement>::template value_t<Model>, NonMappedType<Model, ModelElement>>, char> = 0>
    static auto map(const ModelElement &)
    {
        static_assert(!std::is_same_v<typename table<SearchTag, ModelElement>::template value_t<Model>, NonMappedType<Model, ModelElement>>,
                      "There are no defined mapping for requested type. "
                      "Please implement 'tag::translation::table' specialization for type if you require it");
        return NonMappedType<Model, ModelElement>{};
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

struct has_value
{
    template<class T>
    bool operator()(const T&) { return  true; }

    template<class T>
    bool operator()(const std::optional<T> &) { return  true; }

    template<class ... Args>
    static bool test(const Args&... args)
    {
        return (has_value{}(args) || ... || false);
    }
};


template<class T>
struct decay_optional { using type = T; };
template<class T>
struct decay_optional<std::optional<T>> { using type = T; };

template<class T>
using decay_optional_t = typename decay_optional<T>::type;

template<class Model, class Tag, class ModelElement>
using mapped_tagged_element_t = decay_optional_t<decltype(translation::table_mapper<Model, Tag>::template map(std::declval<ModelElement>()))>;

} // namespace tag
} // namespace elasticsearch
} // namespace v7
} // namespace search

#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_UTILS_HPP
