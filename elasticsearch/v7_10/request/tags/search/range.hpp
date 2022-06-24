#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_RANGE_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_RANGE_HPP

#include "elasticsearch/v7_10/answer_model/search/object/range/Range.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;

template<class Model, class ...SpecificModelParams>
using range = ::model::search::Range<Model, SpecificModelParams...>;

template<class Model, class SpecificModelParam>
using range_element = ::model::search::range::element<Model, SpecificModelParam>;

namespace translation
{
template<class ModelElement>
struct table<model::search::RangeElementTag,
             ModelElement> {
    template<class Model>
    using value_t = range_element<Model, ModelElement>;
};
}


namespace create
{
    template<class Model, class ModelElement,
             template<class> class Cmp>
    range_element<Model, ModelElement>
                range_element_tag(typename ModelElement::value_t &&val)
    {
        return range_element<Model, ModelElement>::template make<Cmp>(std::forward<typename ModelElement::value_t>(val));
    }

    template<class Model, class ModelElement,
             template<class> class Cmp1,
             template<class> class Cmp2>
    range_element<Model, ModelElement>
                range_element_tag(typename ModelElement::value_t &&val1, typename ModelElement::value_t &&val2)
    {
        return range_element<Model, ModelElement>::template make<Cmp1, Cmp2>(std::forward<typename ModelElement::value_t>(val1), std::forward<typename ModelElement::value_t>(val2));
    }

    template<class Model, class ...SpecificModelParam>
    range<Model, SpecificModelParam...>
                range_tag(const std::array<std::string, sizeof...(SpecificModelParam)> &range_in_str, char sep = ',')
    {
        return range<Model, SpecificModelParam...> (range_in_str, sep);
    }

    template<class Model, class ...SpecificModelParam>
    std::optional<range<Model, SpecificModelParam...>>
                range_tag(const std::array<std::optional<std::string>, sizeof...(SpecificModelParam)> &range_in_str, char sep = ',')
    {
        using return_t = range<Model, SpecificModelParam...>;
        using tuple_t = std::tuple<SpecificModelParam...>;
        bool non_empty = elasticsearch::v7::search::tag::has_value::test(range_in_str[elasticsearch::utils::tuple_element_index_v<SpecificModelParam, tuple_t>]...);
        return non_empty ? std::make_optional<return_t>(range_in_str, sep) : std::optional<return_t>();
    }

    template<class Model, class ...SpecificModelParam,
             class = std::enable_if_t<::model::search::all_of_tag<model::search::RangeElementTag,
                                                                  range_element<Model, SpecificModelParam>...>(), int>>
    range<Model, SpecificModelParam...>
                range_tag(const range_element<Model, SpecificModelParam>& ...elems)
    {
        return range<Model, SpecificModelParam...> (elems...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_RANGE_HPP
