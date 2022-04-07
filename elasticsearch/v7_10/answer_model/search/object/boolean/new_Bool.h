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
    using self_t = MustNew<Model, SubContexts...>;
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


    MustNew(const MustNew &src) {
        this->getValue() = src.getValue();
    }

    MustNew(MustNew &&src) {
        this->getValue().swap(src.getValue());
    }

    template<class ...SpecificSubContexts,
                        class = std::enable_if_t<
                                                not std::disjunction_v<
                                                            std::is_same<std::decay_t<SpecificSubContexts>, MustNew>...
                                                                      >
                                                       , int>>
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
                                                        SubContexts...)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const ::model::must_new::SubContextArrayElement<Model, SubContexts...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skip SubContextArrayElement by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };


    // standalone serializier
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename SubContexts::subcontext_serializer_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> ext = out.get_shared_mediator_object();
        aggregator_serializer_type ser(ext);
        base_t:: template format_serialize_impl(ser, tracer);
    }

    // helper for upper level boolean standalone
    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(subserializer_parted_type, Parent, ToJSON,
                                                    must_new::SubContextArrayElement<Model, SubContexts...>,
                                                        SubContexts...)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const ::model::must_new::SubContextArrayElement<Model, SubContexts...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skip SubContextArrayElement by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };
    template<class Parent>
    using subcontext_serializer_type = subserializer_parted_type<Parent>;

    // transparetn serializer
    template<class Parent, template<typename> class ...UpperLevels>
    using serializer_dispatcher_type  = txml::SerializerDispatcher<UpperLevels<Parent>...,
                                                                   serializer_parted_type<Parent>,
                                                                   typename SubContexts::subcontext_serializer_type<Parent>...>;
    template<template<typename> class ...UpperLevels>
    struct parent : public serializer_dispatcher_type<parent<UpperLevels...>, UpperLevels...>
    {
        using base_t = serializer_dispatcher_type<parent<UpperLevels...>, UpperLevels...>;

        parent(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
               std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
        base_t(external_iterators_stack)
        {
        }
    };
};


template<class Model, class ...SubContexts>
class BooleanNew : public txml::XMLNode<BooleanNew<Model, SubContexts...>,
                                        SubContexts...>
{
public:
    using self_t = BooleanNew<Model, SubContexts...>;
    using base_t = txml::XMLNode<BooleanNew<Model, SubContexts...>,
                                 SubContexts...>;

    static constexpr std::string_view class_name()
    {
        return "bool";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    template<class ...BooleanParamsTagsPack>
    BooleanNew(BooleanParamsTagsPack &&...args)
    {
        (this->template set<std::decay_t<BooleanParamsTagsPack>>(std::make_shared<std::decay_t<BooleanParamsTagsPack>>(std::forward<BooleanParamsTagsPack>(args))),...);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               BooleanNew<Model, SubContexts...>,
                                                        SubContexts...) {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename SubContexts::subcontext_serializer_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Parent, template<typename> class ...UpperLevels>
    using serializer_dispatcher_type  = txml::SerializerDispatcher<UpperLevels<Parent>..., serializer_parted_type<Parent>,
                                                                   typename SubContexts::serializer_dispatcher_type<Parent, UpperLevels...>...>;

    //      b) just syntax sugar to reduce template<template<template>> params
    //         because it do not know how to write it down just now...
    template<template<typename> class ...UpperLevels>
    struct parent : public serializer_dispatcher_type<parent<UpperLevels...>, UpperLevels...>
    {
        using base_t = serializer_dispatcher_type<parent<UpperLevels...>, UpperLevels...>;

        parent(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
               std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
        base_t(external_iterators_stack)
        {
        }
    };


    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_impl(ser, tracer);
        //*(out.get_shared_mediator_object()) = ser. template finalize(tracer);
    }
};
}
#endif
