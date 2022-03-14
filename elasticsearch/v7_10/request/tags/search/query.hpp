#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP

#include "elasticsearch/v7_10/answer_model/search/object/boolean/serializer.hpp"
#include "elasticsearch/v7_10/answer_model/search/serializer.hpp"


namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;

template<class Model, class ...SpecificQueryParams>/* Boolean, Must */
struct query
{
    using value_type = query_subrequest<Model, typename std::decay_t<SpecificQueryParams>::value_type...>;
    using query_element_value_type = ::model::Query<Model, typename std::decay_t<SpecificQueryParams>::value_type...>;

    // Only itself serializer must be here... just relief
    //         a) we must declate serialzier for element by itself
    template<class ParentAggregator>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(QueryContextItself,
                                               ParentAggregator, ToJSON,
                                               value_type, query_element_value_type)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const value_type &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skipe query_subrequest by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };
    //      b) must build aggregated serializer for all subsequent serializer hints for all subqueries
    //         we cannot just simply inherits `parent` from subsequent serializers by itseltf, because all of them are
    //         txml::Dispatcher types which are not supposed to be in multiple inheritance
    //      So we must combine each `parent` as tuple element.
    //      Tuple is used as convenient way to unpack variadic bunch and use std::apply - no other reasons to use tuple here
    //      only expand `parent' templates bundle
    struct QueryContextToJSON : public std::tuple<typename SpecificQueryParams::parent<QueryContextItself>...>
    {
        using base_t = std::tuple<typename SpecificQueryParams::parent<QueryContextItself>...>;
        using base_t::base_t;

        QueryContextToJSON(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                           std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            base_t(typename SpecificQueryParams::parent<QueryContextItself>(external_iterators_stack)...)
            {}
    };
    //      c) here it is - whole query serializer (!) but as tuple of subsequent serializers!!!
    //          thus we must use std::apply ultimately
    using serializer_type = QueryContextToJSON;

    /**************/
    template <template<typename> class CustomSerializer>
    struct CustomQueryContextToJSON : public std::tuple<typename SpecificQueryParams::custom_parent<CustomSerializer, QueryContextItself>...>
    {
        using base_t = std::tuple<typename SpecificQueryParams::custom_parent<CustomSerializer, QueryContextItself>...>;
        using base_t::base_t;

        CustomQueryContextToJSON(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                           std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            base_t(typename SpecificQueryParams::custom_parent<CustomSerializer, QueryContextItself>(external_iterators_stack)...)
            {}
    };
    /**************/
    template<class ...QueryParamsTagsPack>
    query(QueryParamsTagsPack &&...args)
    {
        auto query = instance.template emplace<::model::Query<Model, typename std::decay_t<QueryParamsTagsPack>::value_type...>>();
        (query->template set<typename std::decay_t<QueryParamsTagsPack>::value_type>(args.instance_ptr), ...);
    }

    template <class Tracer = txml::EmptyTracer>
    nlohmann::json serialize(Tracer tracer = Tracer()) const
    {
        nlohmann::json js = nlohmann::json::object();;
        this->serialize(js, std::move(tracer));
        return js;
    }

    template <class Tracer = txml::EmptyTracer>
    void serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        serializer_type s;
        // here we go! use apply for tuple of serializers
        std::apply([this, &tracer, &to] (auto &ser) {
            instance.format_serialize(ser, tracer);
            ser. template finalize(to, tracer);
        }, static_cast<typename serializer_type::base_t&>(s));
    }

    template <template<typename> class CustomSerializer, class Tracer = txml::EmptyTracer>
    void custom_serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        CustomQueryContextToJSON<CustomSerializer> s;
        // here we go! use apply for tuple of serializers
        std::apply([this, &tracer, &to] (auto &ser) {
            instance.format_serialize(ser, tracer);
            ser. template finalize(to, tracer);
        }, static_cast<typename CustomQueryContextToJSON<CustomSerializer>::base_t&>(s));
    }

    value_type instance;
};

template<>
struct query<::model::EmptyModel, ::model::EmptyParam>
{
    using value_type = query_subrequest<::model::EmptyModel,::model::EmptyParam>;
    using query_element_value_type = ::model::Query<::model::EmptyModel,::model::EmptyParam>;
    value_type instance;


    template<class ParentAggregator>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(QueryContextItself,
                                               ParentAggregator, ToJSON,
                                               value_type, query_element_value_type)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const value_type &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skipe query_subrequest by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(QueryContextToJSON,
                                             QueryContextItself<QueryContextToJSON>,
                                             MatchAllToJSON<QueryContextToJSON>)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
        QueryContextToJSON(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                           std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
            base_t(external_iterators_stack)
        {
        }
    };

    using serializer_type = QueryContextToJSON;
    query()
    {
        instance.template emplace<query_element_value_type>();
    }
    template <class Tracer = txml::EmptyTracer>
    nlohmann::json serialize(Tracer tracer = Tracer()) const
    {
        nlohmann::json js = nlohmann::json::object();;
        this->serialize(js, std::move(tracer));
        return js;
    }

    template <class Tracer = txml::EmptyTracer>
    void serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        serializer_type s;
        instance.format_serialize(s, tracer);
        s.finalize(to, tracer);
    }
};

using query_all = query<::model::EmptyModel, ::model::EmptyParam>;
namespace create
{
    template<class Model, class ...SpecificQueryParams>
    query<Model, std::decay_t<SpecificQueryParams>...> query_tag(SpecificQueryParams &&...args)
    {
        return query<Model, std::decay_t<SpecificQueryParams>...> (std::forward<SpecificQueryParams>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_QUERY_HPP
