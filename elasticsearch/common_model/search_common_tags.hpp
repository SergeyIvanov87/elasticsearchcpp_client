#ifndef ELASTICSEARCH_COMMON_MODEL_SEARCH_TAG_HPP
#define ELASTICSEARCH_COMMON_MODEL_SEARCH_TAG_HPP

#include "elasticsearch/common_model/model.hpp"
#include "elasticsearch/v7_10/request/tags/search/tags.hpp"

namespace elasticsearch
{
namespace common_model
{
namespace search
{
namespace tag
{
template <class Model, class ModelElement>
inline auto make(const std::optional<ModelElement> &arg)
{
    return elasticsearch::v7::search::tag::make_term<Model>(arg);
}

template <class Model, class ModelElement>
inline auto make(std::optional<ModelElement> &&arg)
{
    return elasticsearch::v7::search::tag::make_term<Model>(std::move(arg));
}

template <class Model>
inline auto make(const std::optional<elasticsearch::common_model::Description> &arg)
{
    using element_t = elasticsearch::common_model::Description;
    return arg.has_value() ?
            std::optional<elasticsearch::v7::search::tag::simple_query_string<
                                Model, element_t>>(
                                    elasticsearch::v7::search::tag::create::simple_query_string_tag<Model, element_t>(arg.value().getValue())) :
            std::optional<elasticsearch::v7::search::tag::simple_query_string<Model, element_t>>();
}

template <class Model>
inline auto make(std::optional<elasticsearch::common_model::Description> &&arg)
{
    return make<Model>(arg);
}

template <class Model>
inline auto make(const std::optional<elasticsearch::common_model::Tags> &arg)
{
    return elasticsearch::v7::search::tag::make_terms<Model>(arg);
}

template <class Model>
inline auto make(std::optional<elasticsearch::common_model::Tags> &&arg)
{
    return elasticsearch::v7::search::tag::make_terms<Model>(std::move(arg));
}

template <class Model, class ModelElement, class ...Args>
inline auto make(Args &&...args)
{
    return make<Model>(std::optional<ModelElement>(std::forward<Args>(args)...));
}
} // namespace tag
} // namespace search
} // namespace common_model
} // namespace elasticsearch
#endif // ELASTICSEARCH_COMMON_MODEL_SEARCH_TAG_HPP
