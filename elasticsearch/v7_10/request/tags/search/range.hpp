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

template<class Model, class SpecificModelParams>
using range_element = ::model::search::Range<Model, SpecificModelParams>;

namespace create
{
    template<class Model, class SpecificModelParam>
    range_element<Model, SpecificModelParam>
                range_tag(const std::string &range_in_str, char sep = ',')
    {
        return range_element<Model, SpecificModelParam> (range_in_str, sep);
    }

    template<class Model, class SpecificModelParam>
    range_element<Model, SpecificModelParam>
                range_tag(const std::optional<std::string> &range_in_str, char sep = ',')
    {
        return range_element<Model, SpecificModelParam> (range_in_str, sep);
    }


    template<class Model, class SpecificModelParam, template<class> class Limit,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Range<Model,
                                                                                                            elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::RangeElementTag, SpecificModelParam>>,
                                                                                     Limit<typename SpecificModelParam::value_t>>()
                                      && ::model::search::all_of_tag<model::search::RangeElementTag, Limit<typename SpecificModelParam::value_t>>(), int>>
    range_element<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::RangeElementTag, SpecificModelParam>>
                range_tag(const Limit<typename SpecificModelParam::value_t> &l)
    {
        return range_element<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::RangeElementTag, SpecificModelParam>> (l);
    }

    template<class Model, class SpecificModelParam, template<class> class Limit1, template <class> class Limit2,
             class = std::enable_if_t<::model::search::details::enable_for_node_args<::model::search::Range<Model,
                                                                                                            elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::RangeElementTag, SpecificModelParam>>,
                                                                                     Limit1<typename SpecificModelParam::value_t>,
                                                                                     Limit2<typename SpecificModelParam::value_t>>()
                                      && ::model::search::all_of_tag<model::search::RangeElementTag,
                                                                     Limit1<typename SpecificModelParam::value_t>,
                                                                     Limit2<typename SpecificModelParam::value_t>>(), int>>
    range_element<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::RangeElementTag, SpecificModelParam>>
                range_tag(const Limit1<typename SpecificModelParam::value_t> &l1,
                          const Limit2<typename SpecificModelParam::value_t> &l2)
    {
        return range_element<Model, elasticsearch::v7::search::tag::mapped_tagged_element_t<Model, model::search::RangeElementTag, SpecificModelParam>> (l1, l2);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_RANGE_HPP
