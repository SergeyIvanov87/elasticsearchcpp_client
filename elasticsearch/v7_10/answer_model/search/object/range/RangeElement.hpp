#ifndef ANSWER_MODEL_SEARCH_RANGE_RANGE_ELEMENT_HPP
#define ANSWER_MODEL_SEARCH_RANGE_RANGE_ELEMENT_HPP

#include <txml/txml_fwd.h>
#include "elasticsearch/utils/traits.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/range/tags.hpp"
#include "elasticsearch/utils/strings.hpp"

namespace model
{
namespace search
{
namespace range
{
using namespace json;

template<class T>
struct GTE : public txml::XMLNodeLeaf<GTE<T>, T>,
             public TagHolder<RangeElementTag>
{
    using base_t = txml::XMLNodeLeaf<GTE<T>, T>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "gte";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class T>
struct GT : public txml::XMLNodeLeaf<GT<T>, T>,
            public TagHolder<RangeElementTag>
{
    using base_t = txml::XMLNodeLeaf<GT<T>, T>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "gt";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class T>
struct LTE : public txml::XMLNodeLeaf<LTE<T>, T>,
             public TagHolder<RangeElementTag>
{
    using base_t = txml::XMLNodeLeaf<LTE<T>, T>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "lte";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class T>
struct LT : public txml::XMLNodeLeaf<LT<T>, T>,
            public TagHolder<RangeElementTag>
{
    using base_t = txml::XMLNodeLeaf<LT<T>, T>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "lt";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<template<class> class Cmp, class ModelElement>
struct CmpWrap : txml::XMLNode<CmpWrap<Cmp, ModelElement>,
                               Cmp<typename ModelElement::value_t>>
{
    using element_t = ModelElement;
    using element_value_t = typename element_t::value_t;

    using base_t = txml::XMLNode<CmpWrap<Cmp, ModelElement>,
                                 Cmp<typename ModelElement::value_t>>;
    using base_t::base_t;
    using cmp_t = Cmp<element_value_t>;

    CmpWrap(const cmp_t& inst)
    {
        this->template emplace<cmp_t>(inst);
    }

    static constexpr std::string_view class_name()
    {
        return cmp_t::class_name();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return cmp_t::class_node_type();
    }

    TXML_DECLARE_SERIALIZER_CLASS(aggregator_serializer_type, ToJSON,
                                             Cmp<typename ModelElement::value_t>)
    {
        TXML_SERIALIZER_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        this->template getValue<cmp_t>()->template format_serialize_impl(ser, tracer);
    }
};

template<class Model, class Element>
class element: public txml::XMLNode<element<Model, Element>,
                                    CmpWrap<GTE, Element>, CmpWrap<GT, Element>,
                                    CmpWrap<LTE, Element>, CmpWrap<LT, Element>>,
                      public TagHolder<RangeElementTag>
{
public:
    using base_t = txml::XMLNode<element<Model, Element>,
                                 CmpWrap<GTE, Element>, CmpWrap<GT, Element>,
                                 CmpWrap<LTE, Element>, CmpWrap<LT, Element>>;
    using element_t = Element;
    using element_value_t = typename element_t::value_t;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<Element>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    // convenient fabric
    template<template<class> class Cmp,
             class ElementValueT = element_value_t>
    static element make(ElementValueT &&val)
    {
        return element(Cmp<element_value_t>(std::forward<ElementValueT>(val)));
    }

    template<template<class> class Cmp1, template<class> class Cmp2,
             class ElementValueT = element_value_t>
    static element make(ElementValueT &&val1, ElementValueT &&val2)
    {
        return element(Cmp1<element_value_t>(std::forward<ElementValueT>(val1)),
                       Cmp2<element_value_t>(std::forward<ElementValueT>(val2)));
    }

    // ctors
    element (const std::optional<std::string> &range_description, char value_sep = ',')
    {
        if (range_description.has_value())
        {
            *this = element(range_description.value(), value_sep);
        }
    }

    element (const std::string &range_description, char value_sep = ',')
    {
        auto range_pair = elasticsearch::utils::get_range_values(range_description, value_sep);
        auto greater = std::get<0>(range_pair);
        if (greater.has_value())
        {
            const auto &gte = greater.value();
            if (gte.border_included)
            {
                this->template emplace<CmpWrap<GTE, Element>>(gte.value);
            }
            else
            {
                this->template emplace<CmpWrap<GTE, Element>>(gte.value);
            }
        }
        auto lower = std::get<1>(range_pair);
        if (lower.has_value())
        {
            const auto &lte = lower.value();
            if (lte.border_included)
            {
                this->template emplace<CmpWrap<LTE, Element>>(lte.value);
            }
            else
            {
                this->template emplace<CmpWrap<LT, Element>>(lte.value);
            }
        }
    }

    template<template<class> class Cmp>
    element (const Cmp<element_value_t> &cmp)
    {
        this->template emplace<CmpWrap<Cmp, Element>>(cmp);
    }

    template<template<class> class Cmp1, template<class> class Cmp2>
    element (const Cmp1<element_value_t> &cmp1, const Cmp2<element_value_t> &cmp2) :
        element(cmp1)
    {
        this->template emplace<CmpWrap<Cmp2, Element>>(cmp2);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                                    /*element<Model, Element>,*/
                                                    CmpWrap<GTE, Element>, CmpWrap<GT, Element>,
                                                    CmpWrap<LTE, Element>, CmpWrap<LT, Element>/*,
                                                    GTE<element_value_t>, GT<element_value_t>,
                                                    LTE<element_value_t>, LT<element_value_t>*/)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };
};
} // namespace range
} // namespace search
} // namespace model
#endif // ANSWER_MODEL_SEARCH_RANGE_RANGE_ELEMENT_HPP
