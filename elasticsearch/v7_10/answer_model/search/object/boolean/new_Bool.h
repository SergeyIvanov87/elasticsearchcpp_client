#ifndef ANSWER_MODEL_SEARCH_BOOLEAN_NEW_BOOL_H
#define ANSWER_MODEL_SEARCH_BOOLEAN_NEW_BOOL_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/search/object/full_text/SimpleQueryString.hpp"

namespace model
{
using namespace json;
namespace must_new {

using namespace json;
template<class Model, class Element>
class ElementToQuery: public txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>
{
public:
    using base_t = txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>;
    using element_t = Element;

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
    using value_t = QueryElement;
    using base_t = txml::XMLNode<Term<Model, value_t>,
                                 ElementToQuery<Model, value_t>>;

    static constexpr std::string_view class_name()
    {
        return "term";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Term(typename value_t::value_t &&v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(std::move(v)));
    }

    Term(const typename value_t::value_t &v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(v));
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(subcontext_serializer_type, Parent, ToJSON,
                                               //Term<Model, value_t>,
                                                    ElementToQuery<Model, value_t>)  {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };
};


template<class Model, class QueryElement>
class Terms : public txml::XMLNode<Terms<Model, QueryElement>,
                                       ElementToQuery<Model, QueryElement>>
{
public:
    using value_t = QueryElement;
    using base_t = txml::XMLNode<Terms<Model, value_t>,
                                 ElementToQuery<Model, value_t>>;

    static constexpr std::string_view class_name()
    {
        return "terms";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Terms(typename value_t::value_t &&v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(std::move(v)));
    }

    Terms(const typename value_t::value_t &v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(v));
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(subcontext_serializer_type, Parent, ToJSON,
                                               //Terms<Model, QueryElement>,
                                                    ElementToQuery<Model, value_t>)  {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };
};

template<class Model, class ...SubContexts>
class SubContextArrayElement : public txml::XMLNode<SubContextArrayElement<Model, SubContexts...>,
                                                       SubContexts...>
{
public:
    using base_t = txml::XMLNode<SubContextArrayElement<Model, SubContexts...>,
                                 SubContexts...>;

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

template<class Model, class ...SubContexts>
class MustNew: public txml::XMLArray<MustNew<Model, SubContexts...>,
                                              must_new::SubContextArrayElement<Model, SubContexts...>>
{
public:
    using element_t = must_new::SubContextArrayElement<Model, SubContexts...>;
    using base_t = txml::XMLArray<MustNew<Model, SubContexts...>,
                                  must_new::SubContextArrayElement<Model, SubContexts...>>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "must";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    template<class ...SpecificSubContexts>
    MustNew(SpecificSubContexts && ...args) {
        auto elem = std::make_shared<element_t>();
        (elem->template emplace <SpecificSubContexts>(std::forward<SpecificSubContexts>(args)), ...);
        this->getValue().push_back(elem);
    }


    // Subcontext Array element constitute a lot of variadic subitem templates : Term, Terms, QuerySimpleString and something other
    // Many of them require its own variadic templates. So it's it not possible to enumerate several variadic packs here in standalone serializer
    // To reduce variadic packs on one we should declare serializer as part of class
    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               MustNew<Model, SubContexts...>,
                                                    must_new::SubContextArrayElement<Model, SubContexts...>,
                                                        SubContexts.../*,
                                                        must_new::ElementToQuery<Model, typename SubContexts::value_t>...*/)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const ::model::must_new::SubContextArrayElement<Model, SubContexts...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skip SubContextArrayElement by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };
/*
    template<template<class, class> class ...SubContextSerializers>
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type<Model, SubContext...>>,
                                             SubContextSerializers<aggregator_serializer_type<Model, SubContext...>, Model>)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
        aggregator_serializer_type(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                                   std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            base_t(external_iterators_stack)
        {
        }
    }; */
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename SubContexts::subcontext_serializer_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
        aggregator_serializer_type(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                                   std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            base_t(external_iterators_stack)
        {
        }
    };
};
}
#endif
