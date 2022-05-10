#ifndef ANSWER_MODEL_SEARCH_RANGE_RANGE_HPP
#define ANSWER_MODEL_SEARCH_RANGE_RANGE_HPP

#include "elasticsearch/v7_10/answer_model/search/object/range/RangeElement.hpp"

namespace model
{
namespace search
{
template<class Model, class Element>
class Range : public txml::XMLNode<Range<Model, Element>,
                                   range::ElementToQuery<Model, Element>>,
                public TagHolder<QueryElementTag>
{
public:
    using self_t = Range<Model, Element>;
    using element_t = range::ElementToQuery<Model, Element>;
    using base_t = txml::XMLNode<Range<Model, Element>,
                                 element_t>;

    static constexpr std::string_view class_name()
    {
        return "range";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }


    Range(const Range &src)
    {
        this->getValue() = src.getValue();
    }

    Range(Range &&src)
    {
        this->getValue().swap(src.getValue());
    }

    template<class ...RangeTags, class =
             std::enable_if_t<details::enable_for_node_args<Range, RangeTags...>()
                              && all_of_tag<RangeElementTag, RangeTags...>()
                              || (/*std::is_same_v<typename std::integral_constant<int, sizeof...(RangeTags)>::type,
                                                typename std::integral_constant<int, 1>::type>
!!!!!is_trivially_constructible

                                  &&*/ !std::conjunction_v<std::is_same<std::decay_t<RangeTags>, Range>...>), int>>
    Range(RangeTags && ...args) {
        this->template emplace<element_t>(std::forward<RangeTags>(args)...);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               Range<Model, Element>,
                                               element_t) {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename element_t::serializer_parted_type<aggregator_serializer_type>)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_impl(ser, tracer);
    }
};
} // namespace search
} // namespace model
#endif // ANSWER_MODEL_SEARCH_RANGE_RANGE_HPP
