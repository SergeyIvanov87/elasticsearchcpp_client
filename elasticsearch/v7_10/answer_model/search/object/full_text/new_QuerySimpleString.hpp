#ifndef ANSWER_MODEL_SEARCH_FULL_TEXT_SIMPLE_QUERY_STRING_NEW_HPP
#define ANSWER_MODEL_SEARCH_FULL_TEXT_SIMPLE_QUERY_STRING_NEW_HPP

#include <txml/txml_fwd.h>

namespace model
{
namespace full_text_new
{
class Query: public txml::XMLNodeLeaf<Query, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Query, std::string>;

    static constexpr std::string_view class_name()
    {
        return "query";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Query(const typename base_t::value_t& value) :
        base_t(value)
    {
    }
};

//
template<class Model, class Element>
class Fields : public txml::XMLNodeLeafNoData<Fields<Model, Element>>
{
public:
    using base_t = txml::XMLNodeLeafNoData<Fields<Model, Element>>;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<Element>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class Model, class ...SortedElements>
class FieldsArrayElement : public txml::XMLNode<FieldsArrayElement<Model, SortedElements...>,
                                                       Fields<Model, SortedElements>...>
{
public:
    using base_t = txml::XMLNode<FieldsArrayElement<Model, SortedElements...>,
                                 Fields<Model, SortedElements>...>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    FieldsArrayElement()
    {
        (this->template emplace<Fields<Model, SortedElements>>(), ...);
    }
};
template<class Model, class ...FieldsElements>
class FieldsArray: public txml::XMLArray<FieldsArray<Model, FieldsElements...>,
                                         FieldsArrayElement<Model, FieldsElements...>>
{
public:
    using element_t = FieldsArrayElement<Model, FieldsElements...>;
    using base_t = txml::XMLArray<FieldsArray<Model, FieldsElements...>,
                                  FieldsArrayElement<Model, FieldsElements...>>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "fields";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    FieldsArray() :
        base_t({std::make_shared<element_t>()})
    {
    }
};
///
template<class Model, class ...FieldsElements>
class SimpleQueryString : public txml::XMLNode<SimpleQueryString<Model, FieldsElements...>,
                                               Query,
                                               FieldsArray<Model, FieldsElements...>>
{
public:
    using self_t = BooleanNew<Model, FieldsElements...>;
    using base_t = txml::XMLNode<SimpleQueryString<Model, FieldsElements...>,
                                                   Query,
                                                   FieldsArray<Model, FieldsElements...>>;

    static constexpr std::string_view class_name()
    {
        return "simple_query_string";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    SimpleQueryString(const std::string &query_string) {
        this->template set(std::make_shared<FieldsArray<Model, FieldsElements...>>());
        this->template emplace<Query>(query_string);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               SimpleQueryString<Model, FieldsElements...>,
                                                    Query,
                                                    FieldsArray<Model, FieldsElements...>,
                                                        FieldsArrayElement<Model, FieldsElements...>,
                                                            Fields<Model, FieldsElements>...)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const ::model::full_text_new::FieldsArrayElement<Model, FieldsElements...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skipe FieldsArrayElement by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                                     serializer_parted_type<aggregator_serializer_type>)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
/*
        aggregator_serializer_type(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                                   std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            json::SerializerCore(external_iterators_stack),
            base_t(external_iterators_stack)
        {
        }*/
    };

    template<class Parent>
    using subcontext_serializer_type = serializer_parted_type<Parent>;

    template<class Parent>
    using subcontext_virtual_serializer_type = serializer_parted_type<Parent>;

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_impl(ser, tracer);
        //*(out.get_shared_mediator_object()) = ser. template finalize(tracer);
    }
};
}
}
#endif // ANSWER_MODEL_SEARCH_FULL_TEXT_SIMPLE_QUERY_STRING_NEW_HPP
