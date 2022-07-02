#ifndef ANSWER_MODEL_SEARCH_GEO_GEO_BOUNDING_BOX_H
#define ANSWER_MODEL_SEARCH_GEO_GEO_BOUNDING_BOX_H

#include "elasticsearch/v7_10/answer_model/search/object/geo/GeoBoundingBoxElement.hpp"

namespace model
{
namespace search
{
template<class Model, class Element>
class GeoBoundingBox : public txml::XMLNode<GeoBoundingBox<Model, Element>,
                                            geo::ElementToQuery<Model, Element>>,
                public TagHolder<QueryElementTag, FilterElementTag>
{
public:
    using self_t = GeoBoundingBox<Model, Element>;
    using element_t = geo::ElementToQuery<Model, Element>;
    using base_t = txml::XMLNode<GeoBoundingBox<Model, Element>,
                                 element_t>;

    static constexpr std::string_view class_name()
    {
        return "geo_bounding_box";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    GeoBoundingBox(const element_t &bounding_box_value)
    {
        this->template emplace<element_t>(bounding_box_value);
    }

    GeoBoundingBox (const geo::BBTopLeft &t_l, const geo::BBBottomRight &b_r)
    {
        this->template emplace<element_t>(t_l, b_r);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               GeoBoundingBox<Model, Element>,
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
    void format_serialize_request(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_request(ser, tracer);
    }
};
} // namespace search
} // namespace model
#endif
