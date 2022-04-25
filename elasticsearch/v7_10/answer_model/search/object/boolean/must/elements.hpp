#ifndef ANSWER_MODEL_SEARCH_BOOLEAN_MUST_ELEMENTS_H
#define ANSWER_MODEL_SEARCH_BOOLEAN_MUST_ELEMENTS_H

#include <txml/txml_fwd.h>
#include "elasticsearch/utils/traits.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/tags.hpp"

namespace model
{
namespace search
{
namespace must
{
using namespace json;
template<class Model, class Element>
class ElementToQuery: public txml::XMLNodeLeaf<ElementToQuery<Model, Element>,
                                               typename Element::value_t>
{
public:
    using base_t = txml::XMLNodeLeaf<ElementToQuery<Model, Element>,
                                     typename Element::value_t>;
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
                                       ElementToQuery<Model, QueryElement>>,
             public TagHolder<MustElementTag>
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

    Term(value_t &&v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(std::move(v.getValue())));
    }

    Term(const value_t &v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(v.getValue()));
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               //Term<Model, value_t>,
                                                    ElementToQuery<Model, value_t>)  {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };
};


template<class Model, class QueryElement>
class Terms : public txml::XMLNode<Terms<Model, QueryElement>,
                                       ElementToQuery<Model, QueryElement>>,
              public TagHolder<MustElementTag>
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

    Terms(value_t &&v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(std::move(v.getValue())));
    }

    Terms(const value_t &v) {
        this->template set (std::make_shared<ElementToQuery<Model, value_t>>(v.getValue()));
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
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
} // namespace must


using namespace json;
template<class Model, class ...SubContexts>
class Must: public txml::XMLArray<Must<Model, SubContexts...>,
                                  must::SubContextArrayElement<Model, SubContexts...>>,
            public TagHolder<BooleanElementTag>
{
public:
    using element_t = must::SubContextArrayElement<Model, SubContexts...>;
    using self_t = Must<Model, SubContexts...>;
    using base_t = txml::XMLArray<Must<Model, SubContexts...>,
                                  must::SubContextArrayElement<Model, SubContexts...>>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "must";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }


    Must(const Must &src) {
        this->getValue() = src.getValue();
    }

    Must(Must &&src) {
        this->getValue().swap(src.getValue());
    }

    template<class ...SpecificSubContexts, class =
             std::enable_if_t<details::enable_for_node_args<Must, SpecificSubContexts...>()
                              && all_of_tag<MustElementTag, SpecificSubContexts...>(), int>>
    Must(SpecificSubContexts && ...args) {
        auto elem = std::make_shared<element_t>();
        (elem->template emplace <SpecificSubContexts>(std::forward<SpecificSubContexts>(args)), ...);
        this->getValue().push_back(elem);
    }

    template<class ...SpecificSubContexts, class =
             std::enable_if_t<details::enable_for_node_args<Must, SpecificSubContexts...>(), int>>
    Must(const std::optional<SpecificSubContexts>&...args)
    {
        static_assert(all_of_tag<MustElementTag, SpecificSubContexts...>(),
                      "Must creation from std::optional assumes list of Node Element");
        auto elem = std::make_shared<element_t>();
        ( (args.has_value() ? elem->template emplace <SpecificSubContexts>(args.value()),true : false), ...);
        this->getValue().push_back(elem);
    }

    // Subcontext Array element constitute a lot of variadic subitem templates : Term, Terms, QuerySimpleString and something other
    // Many of them require its own variadic templates. So it's it not possible to enumerate several variadic packs here in standalone serializer
    // To reduce variadic packs on one we should declare serializer as part of class
    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               Must<Model, SubContexts...>,
                                                    must::SubContextArrayElement<Model, SubContexts...>,
                                                        SubContexts...)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const must::SubContextArrayElement<Model, SubContexts...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skip SubContextArrayElement by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };


    // standalone serializier
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename SubContexts::serializer_parted_type<aggregator_serializer_type>...)
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
};
} // namespace search
} // namespace model
#endif // ANSWER_MODEL_SEARCH_BOOLEAN_MUST_ELEMENTS_H
