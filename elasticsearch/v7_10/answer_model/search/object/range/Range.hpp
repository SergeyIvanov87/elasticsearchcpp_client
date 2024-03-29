#ifndef ANSWER_MODEL_SEARCH_RANGE_RANGE_HPP
#define ANSWER_MODEL_SEARCH_RANGE_RANGE_HPP

#include "elasticsearch/v7_10/answer_model/search/object/range/RangeElement.hpp"

namespace model
{
namespace search
{
template<class Model, class ...Element>
class Range : public txml::XMLNode<Range<Model, Element...>,
                                   range::element<Model, Element>...>,
                public TagHolder<QueryElementTag, MustElementTag>
{
    using tuple_t = std::tuple<range::element<Model, Element>...>;
public:
    using self_t = Range<Model, Element...>;
    using base_t = txml::XMLNode<Range<Model, Element...>,
                                 range::element<Model, Element>...>;
    using base_t::base_t;
    static constexpr std::string_view class_name()
    {
        return "range";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    template<size_t N>
    Range(const std::array<std::string, N> &arr, char sep = ',')
    {
        static_assert(sizeof...(Element) == N, "string array must be the same size as elements in ranges");
        (this->template emplace<range::element<Model, Element>>(arr[elasticsearch::utils::tuple_element_index_v<range::element<Model, Element>, tuple_t>], sep), ...);
    }

    template<size_t N>
    Range(const std::array<std::optional<std::string>, N> &arr, char sep = ',')
    {
        static_assert(sizeof...(Element) == N, "optional string array must be the same size as elements in ranges");
        ((arr[elasticsearch::utils::tuple_element_index_v<range::element<Model, Element>, tuple_t>].has_value() ?
            (this->template emplace<range::element<Model, Element>>(arr[elasticsearch::utils::tuple_element_index_v<range::element<Model, Element>, tuple_t>], sep),true)
            : (false)), ...);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               Range<Model, Element...>,
                                               range::element<Model, Element>...) {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename range::element<Model, Element>::template serializer_parted_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_request(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_request(ser, tracer);
    }
};
} // namespace search
} // namespace model
#endif // ANSWER_MODEL_SEARCH_RANGE_RANGE_HPP
