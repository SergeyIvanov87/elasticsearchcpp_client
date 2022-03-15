#ifndef ANSWER_MODEL_SEARCH_BOOLEAN_BOOL_H
#define ANSWER_MODEL_SEARCH_BOOLEAN_BOOL_H

#include <txml/txml_fwd.h>
namespace model
{
namespace must {
template<class Model, class Element>
class ElementToQuery: public txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>
{
public:
    using base_t = txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<Element>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ElementToQuery(const typename Element::value_t& value) :
        base_t(value)
    {
    }
};

template<class Model, class QueryElement>
class Term : public txml::XMLNode<Term<Model, QueryElement>,
                                       ElementToQuery<Model, QueryElement>>
{
public:
    using base_t = txml::XMLNode<Term<Model, QueryElement>,
                                 ElementToQuery<Model, QueryElement>>;

    static constexpr std::string_view class_name()
    {
        return "term";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


template<class Model, class QueryElement>
class Terms : public txml::XMLNode<Terms<Model, QueryElement>,
                                       ElementToQuery<Model, QueryElement>>
{
public:
    using base_t = txml::XMLNode<Terms<Model, QueryElement>,
                                 ElementToQuery<Model, QueryElement>>;

    static constexpr std::string_view class_name()
    {
        return "terms";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


template<class Model, class ...SortedElements>
class SpecificQueryArrayElement : public txml::XMLNode<SpecificQueryArrayElement<Model, SortedElements...>,
                                                       Term<Model, SortedElements>...,
                                                       Terms<Model, SortedElements>...>
{
public:
    using base_t = txml::XMLNode<SpecificQueryArrayElement<Model, SortedElements...>,
                                 Term<Model, SortedElements>...,
                                 Terms<Model, SortedElements>...>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}

template<class Model, class ...SortedElements>
class Must: public txml::XMLArray<Must<Model, SortedElements...>,
                                              must::SpecificQueryArrayElement<Model, SortedElements...>>
{
public:
    using element_t = must::SpecificQueryArrayElement<Model, SortedElements...>;
    using base_t = txml::XMLArray<Must<Model, SortedElements...>,
                                  must::SpecificQueryArrayElement<Model, SortedElements...>>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "must";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


namespace filter {
template<class Model, class Element>
class ElementToQuery: public txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>
{
public:
    using base_t = txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<Element>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ElementToQuery(const typename Element::value_t& value) :
        base_t(value)
    {
    }
};

template<class Model, class QueryElement>
class Term : public txml::XMLNode<Term<Model, QueryElement>,
                                       ElementToQuery<Model, QueryElement>>
{
public:
    using base_t = txml::XMLNode<Term<Model, QueryElement>,
                                 ElementToQuery<Model, QueryElement>>;

    static constexpr std::string_view class_name()
    {
        return "term";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


template<class Model, class ...SortedElements>
class SpecificQueryArrayElement : public txml::XMLNode<SpecificQueryArrayElement<Model, SortedElements...>,
                                                       Term<Model, SortedElements>...>
{
public:
    using base_t = txml::XMLNode<SpecificQueryArrayElement<Model, SortedElements...>,
                                 Term<Model, SortedElements>...>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}

template<class Model, class ...SortedElements>
class Filter: public txml::XMLArray<Filter<Model, SortedElements...>,
                                              filter::SpecificQueryArrayElement<Model, SortedElements...>>
{
public:
    using element_t = filter::SpecificQueryArrayElement<Model, SortedElements...>;
    using base_t = txml::XMLArray<Filter<Model, SortedElements...>,
                                  filter::SpecificQueryArrayElement<Model, SortedElements...>>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "filter";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class Model, class ...SubContexts>
class Boolean : public txml::XMLNode<Boolean<Model, SubContexts...>,
                                     SubContexts...>
{
public:
    using base_t = txml::XMLNode<Boolean<Model, SubContexts...>,
                                 SubContexts...>;

    static constexpr std::string_view class_name()
    {
        return "bool";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif
