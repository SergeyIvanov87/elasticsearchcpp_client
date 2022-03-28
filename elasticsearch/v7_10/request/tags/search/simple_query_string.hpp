#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SIMPLE_QUERY_STRING_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SIMPLE_QUERY_STRING_HPP

#include "elasticsearch/v7_10/answer_model/search/object/full_text/serializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;
// the same as must
template<class Model, class ...SpecificModelParams>
struct simple_query_string {
    using value_type = ::model::full_text::SimpleQueryString<Model, SpecificModelParams...>;
    using array_type_value_type = ::model::full_text::FieldsArray<Model, SpecificModelParams...>;
    using serializer_type = QuerySimpleQueryStringToJSON<Model, SpecificModelParams...>;

    template<class Parent>
    using serializer_parted_type = QuerySimpleQueryStringToJSONParted<Parent, Model, SpecificModelParams...>;

    template<class Parent, template<typename> class CustomSerializer>
    using custom_serializer_parted_type = /*???*/ QuerySimpleQueryStringToJSONParted<Parent, Model, SpecificModelParams...>;

    template<class Parent, template<typename> class ...UpperLevels>
    using serializer_dispatcher_type  = txml::SerializerDispatcher<UpperLevels<Parent>..., serializer_parted_type<Parent>>;

    template<template<typename>class CustomSerializer, class Parent, template<typename> class ...UpperLevels>
    using custom_serializer_dispatcher_type  = txml::SerializerDispatcher<UpperLevels<Parent>..., serializer_parted_type<Parent>,
                                                                   custom_serializer_parted_type<Parent, CustomSerializer>>;

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

    ////////////
    template<template<typename> class CustomSerializer, template<typename> class ...UpperLevels>
    struct custom_parent : public custom_serializer_dispatcher_type<CustomSerializer, custom_parent<CustomSerializer, UpperLevels...>, UpperLevels...>
    {
        using base_t = custom_serializer_dispatcher_type<CustomSerializer, custom_parent<CustomSerializer, UpperLevels...>, UpperLevels...>;

        custom_parent(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
               std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
        base_t(external_iterators_stack)
        {
        }
    };

    simple_query_string()
    {
        auto elem = std::make_shared<array_type_value_type>();
        instance_ptr = std::make_shared<value_type>();
        instance_ptr->template set(elem);
    }

    template <class Tracer = txml::EmptyTracer>
    void serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        serializer_type s;
        instance_ptr->format_serialize(s, tracer);
        s.finalize(to, tracer);
    }


    template <class Tracer = txml::EmptyTracer>
    nlohmann::json serialize(Tracer tracer = Tracer()) const
    {
        nlohmann::json js = nlohmann::json::object();
        this->serialize(js, std::move(tracer));
        return js;
    }
    std::shared_ptr<value_type> instance_ptr;
};

namespace create
{
    template<class Model, class ...SpecificModelParams>
    simple_query_string<Model, SpecificModelParams...> simple_query_string_tag()
    {
        return simple_query_string<Model, SpecificModelParams...> {};
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SIMPLE_QUERY_STRING_HPP
