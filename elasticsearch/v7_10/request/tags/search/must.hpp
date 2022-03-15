#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP

#include <optional>

#include "elasticsearch/v7_10/answer_model/search/object/boolean/serializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;

// Simple Must Tag, which used to construct `must` object.
// must object declare match conditions for different `SpecificModelParams` elements,
// which parts of `Model`
// So, must incapsulate builder functionality to match `SpecificModelParams` in `Model` set
namespace details
{
    class TermArg {};
    class TermsArg {};
}
template<class T, class Base>
struct RArg : Base {
    using value_t = T;
    RArg(T &&v) : m_arg(std::move(v)) {}
    T&& m_arg;
};
template<class T, class Base>
struct LArg : Base {
    using value_t = T;
    LArg(const T &v) : m_arg(v) {}
    const T& m_arg;
};

template <class T>
auto make_term(T &&arg)
{
    if constexpr (std::is_rvalue_reference_v<decltype(arg)>)
    {
        return RArg<typename std::decay<T>::type, details::TermArg>(std::move(arg));
    }
    else
    {
        return LArg<typename std::decay<T>::type, details::TermArg>(std::forward<T>(arg));
    }
}

template <class T>
auto make_terms(T &&arg)
{
    if constexpr (std::is_rvalue_reference_v<decltype(arg)>)
    {
        return RArg<typename std::decay<T>::type, details::TermsArg>(std::move(arg));
    }
    else
    {
        return LArg<typename std::decay<T>::type, details::TermsArg>(std::forward<T>(arg));
    }
}

template<class Model, class ...SpecificModelParams>
struct must
{
    using value_type = ::model::Must<Model, SpecificModelParams...>;
    using must_array_type_value_type = typename value_type::element_t;

    // standalone itself serializer
    using serializer_type = QueryMustContextToJSON<Model, SpecificModelParams...>;

    // serializer hint: how to serialize this underlying tag for consequent upperlevel tag serializer
    template<class Parent>
    using serializer_parted_type = QueryMustContextToJSONParted<Parent, Model, SpecificModelParams...>;

    template<class Parent, template<typename> class CustomSerializer>
    using custom_serializer_parted_type = /*???*/ QueryMustContextToJSONParted<Parent, Model, SpecificModelParams...>;

    template<class Parent>
    using magic_custom_serializer_parted_type = /*???*/ MagicQueryMustContextToJSONParted<Parent, Model, SpecificModelParams...>;

    template<template<typename> class CustomSerializer>
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(MustAggregatorSerializer,
                                             magic_custom_serializer_parted_type<MustAggregatorSerializer<CustomSerializer>>,
                                             CustomSerializer<MustAggregatorSerializer<CustomSerializer>>)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
        MustAggregatorSerializer(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                           std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            base_t(external_iterators_stack)
        {
        }
    };
    must(typename SpecificModelParams::value_t &&...args)
    {
        auto elem = std::make_shared<must_array_type_value_type>();
        (elem->template emplace<::model::must::Term<Model, SpecificModelParams>>()->template emplace<::model::must::ElementToQuery<Model, SpecificModelParams>>(std::forward<typename SpecificModelParams::value_t>(args)), ...);

        instance_ptr = std::make_shared<value_type>(std::initializer_list<std::shared_ptr<must_array_type_value_type>>{elem});
    }

    must(std::optional<typename SpecificModelParams::value_t> &&...args)
    {
        auto elem = std::make_shared<must_array_type_value_type>();
        ((args.has_value() ? elem->template emplace<::model::must::Term<Model, SpecificModelParams>>()->template emplace<::model::must::ElementToQuery<Model, SpecificModelParams>>(std::forward<typename SpecificModelParams::value_t>(args.value())),false : true), ...);

        instance_ptr = std::make_shared<value_type>(std::initializer_list<std::shared_ptr<must_array_type_value_type>>{elem});
    }


    template<class ...TermWrapper>
    must(TermWrapper&&...args)
    {
        auto elem = std::make_shared<must_array_type_value_type>();

        auto append = [&elem](auto&& val, auto is_same) mutable {
            if (is_same)//std::is_same_v<std::decay_t<decltype(is_same)>, std::true_type>)
            {
                elem->template emplace<::model::must::Term<Model, std::decay_t<decltype(val)>>>()->template emplace<::model::must::ElementToQuery<Model, std::decay_t<decltype(val)>>>(val.getValue());
            }
            else //if constexpr(std::is_same_v<std::decay_t<decltype(is_same)>, std::false_type>)
            {
                elem->template emplace<::model::must::Terms<Model, std::decay_t<decltype(val)>>>()->template emplace<::model::must::ElementToQuery<Model, std::decay_t<decltype(val)>>>(val.getValue());
            }
        };
        (append(args.m_arg, std::is_base_of_v<details::TermArg, std::decay_t<TermWrapper>>), ...);
        instance_ptr = std::make_shared<value_type>(std::initializer_list<std::shared_ptr<must_array_type_value_type>>{elem});
    }

    std::shared_ptr<value_type> instance_ptr;

    template <class Tracer = txml::EmptyTracer>
    void serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        serializer_type s;
        instance_ptr->format_serialize(s, tracer);
        s.finalize(to, tracer);
    }

    template <template<typename> class CustomSerializer, class Tracer = txml::EmptyTracer>
    void custom_serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        MustAggregatorSerializer<CustomSerializer> s;
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
};
namespace create
{
    template<class Model, class ...SpecificModelParams>
    must<Model, SpecificModelParams...> must_tag(typename SpecificModelParams::value_t &&...args)
    {
        return must<Model, SpecificModelParams...> (std::forward<typename SpecificModelParams::value_t>(args)...);
    }

    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(std::optional<typename SpecificModelParams::value_t> &&...args)
    {
        return must<SpecificModelParams...> (std::forward<std::optional<typename SpecificModelParams::value_t>>(args)...);
    }

    template<class Model, class ...SpecificModelParams>
    must<Model, typename SpecificModelParams::value_t...> must_tag_term(SpecificModelParams &&...args)
    {
        return must<Model, typename SpecificModelParams::value_t...> (std::forward<SpecificModelParams>(args)...);
    }
/*
    struct FooI {using value_t = int; value_t val;};
    struct FooF {using value_t = float; value_t val;};
    struct FooS {using value_t = std::string; value_t val;};
    inline void foo()
    {
       must<FooI, FooF, FooS> f(make_term<FooI>(FooI{0}), make_term<FooF>(FooF{0.0f}), make_term<FooS>(FooS{std::string("ss")}));
       (void)f;

    }*/
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
