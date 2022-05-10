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
class GTE : public txml::XMLNodeLeaf<GTE<T>, T>,
            public TagHolder<RangeElementTag>
{
public:
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
class GT : public txml::XMLNodeLeaf<GT<T>, T>,
           public TagHolder<RangeElementTag>
{
public:
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
class LTE : public txml::XMLNodeLeaf<LTE<T>, T>,
            public TagHolder<RangeElementTag>
{
public:
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
class LT : public txml::XMLNodeLeaf<LT<T>, T>,
           public TagHolder<RangeElementTag>
{
public:
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

template<class Model, class Element>
class ElementToQuery: public txml::XMLNode<ElementToQuery<Model, Element>,
                                           GTE<typename Element::value_t>, GT<typename Element::value_t>,
                                           LTE<typename Element::value_t>, LT<typename Element::value_t>>,
                      public TagHolder<RangeElementTag>
{
public:
    using base_t = txml::XMLNode<ElementToQuery<Model, Element>,
                                 GTE<typename Element::value_t>, GT<typename Element::value_t>,
                                 LTE<typename Element::value_t>, LT<typename Element::value_t>>;
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

    ElementToQuery (const std::optional<std::string> &range_description, char value_sep = ',')
    {
        if (range_description.has_value())
        {
            *this = ElementToQuery(range_description.value(), value_sep);
        }
    }

    ElementToQuery (const std::string &range_description, char value_sep = ',')
    {
        auto range_pair = elasticsearch::utils::get_range_values(range_description, value_sep);
        auto greater = std::get<0>(range_pair);
        if (greater.has_value())
        {
            const auto &gte = greater.value();
            if (gte.border_included)
            {
                this->template emplace<GTE<element_value_t>>(gte.value);
            }
            else
            {
                this->template emplace<GTE<element_value_t>>(gte.value);
            }
        }
        auto lower = std::get<1>(range_pair);
        if (lower.has_value())
        {
            const auto &lte = lower.value();
            if (lte.border_included)
            {
                this->template emplace<LTE<element_value_t>>(lte.value);
            }
            else
            {
                this->template emplace<LT<element_value_t>>(lte.value);
            }
        }
    }

    ElementToQuery (const GTE<element_value_t> &gte)
    {
        this->template emplace<GTE<element_value_t>>(gte);
    }
    ElementToQuery (const GT<element_value_t> &gt)
    {
        this->template emplace<GT<element_value_t>>(gt);
    }

    ElementToQuery (const LTE<element_value_t> &lte)
    {
        this->template emplace<LTE<element_value_t>>(lte);
    }
    ElementToQuery (const LT<element_value_t> &lte)
    {
        this->template emplace<LT<element_value_t>>(lte);
    }

    ElementToQuery (const GTE<element_value_t> &gte, const LTE<element_value_t> &lte)
    {
        this->template emplace<GTE<element_value_t>>(gte);
        this->template emplace<LTE<element_value_t>>(lte);
    }

    ElementToQuery (const GT<element_value_t> &gt, const LTE<element_value_t> &lte)
    {
        this->template emplace<GT<element_value_t>>(gt);
        this->template emplace<LTE<element_value_t>>(lte);
    }

    ElementToQuery (const GTE<element_value_t> &gte, const LT<element_value_t> &lt)
    {
        this->template emplace<GTE<element_value_t>>(gte);
        this->template emplace<LT<element_value_t>>(lt);
    }

    ElementToQuery (const GT<element_value_t> &gte, const LT<element_value_t> &lt)
    {
        this->template emplace<GT<element_value_t>>(gte);
        this->template emplace<LT<element_value_t>>(lt);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                                    /*ElementToQuery<Model, Element>,*/
                                                        GTE<element_value_t>, GT<element_value_t>,
                                                        LTE<element_value_t>, LT<element_value_t>)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };
};
} // namespace range
} // namespace search
} // namespace model
#endif // ANSWER_MODEL_SEARCH_RANGE_RANGE_ELEMENT_HPP
