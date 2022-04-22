#ifndef ANSWER_MODEL_SEARCH_GEO_GEO_BOUNDING_BOX_ELEMENT_HPP
#define ANSWER_MODEL_SEARCH_GEO_GEO_BOUNDING_BOX_ELEMENT_HPP

#include <txml/txml_fwd.h>
#include "elasticsearch/utils/traits.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/geo/tags.hpp"

namespace model
{
namespace search
{
namespace geo
{
using namespace json;

class Latitude : public txml::XMLNodeLeaf<Latitude, float>,
                 public TagHolder<GeoElementTag>
{
public:
    using base_t = txml::XMLNodeLeaf<Latitude, float>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "lat";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


class Longitude : public txml::XMLNodeLeaf<Longitude, float>,
                  public TagHolder<GeoElementTag>
{
public:
    using base_t = txml::XMLNodeLeaf<Longitude, float>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "lon";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


class BBTopLeft : public txml::XMLNode<BBTopLeft,
                                       Latitude, Longitude>,
                  public TagHolder<GeoElementTag>
{
public:
    using base_t = txml::XMLNode<BBTopLeft,
                                 Latitude, Longitude>;

    static constexpr std::string_view class_name()
    {
        return "top_left";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    BBTopLeft(const Latitude &lat, const Longitude &lon)
    {
        this->template emplace<Latitude>(lat);
        this->template emplace<Longitude>(lon);
    }
};

class BBBottomRight : public txml::XMLNode<BBBottomRight,
                                           Latitude, Longitude>,
                      public TagHolder<GeoElementTag>
{
public:
    using base_t = txml::XMLNode<BBBottomRight,
                                 Latitude, Longitude>;

    static constexpr std::string_view class_name()
    {
        return "bottom_right";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    BBBottomRight(const Latitude &lat, const Longitude &lon)
    {
        this->template emplace<Latitude>(lat);
        this->template emplace<Longitude>(lon);
    }
};


template<class Model, class Element>
class ElementToQuery: public txml::XMLNode<ElementToQuery<Model, Element>,
                                           BBTopLeft, BBBottomRight>,
                      public TagHolder<GeoElementTag>
{
public:
    using base_t = txml::XMLNode<ElementToQuery<Model, Element>,
                                 BBTopLeft, BBBottomRight>;
    using element_t = Element;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<Element>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ElementToQuery (const BBTopLeft &t_l, const BBBottomRight &b_r)
    {
        this->template emplace<BBTopLeft>(t_l);
        this->template emplace<BBBottomRight>(b_r);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                                    /*ElementToQuery<Model, Element>,*/
                                                        BBTopLeft, BBBottomRight,
                                                        Latitude, Longitude)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };
};
} // namespace geo
} // namespace search
} // namespace model
#endif // ANSWER_MODEL_SEARCH_GEO_GEO_BOUNDING_BOX_ELEMENT_HPP
