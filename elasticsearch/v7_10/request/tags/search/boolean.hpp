#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP

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

// Boolean Tag is a complex builder over Must and Filter (at least) and has some tricky implementation:
// It must carry out C++ multipel variadic templates pack declaration limitation...
// Boolean build aggregate Must<Args1...> and Filter<Args2...> subqueries so it MUST be constructed over
// Args1... and Args2...
// BUT it is impossible to pass TWO or more  variadic templates pack here into Boolean<Args1..., Args2...>
// So, boolean just deducted from `SpecificBooleanParams...`  templates variadic pack which carries
// Must<Args1...> and Filter<Args2...> and deducted in result as Boolean<Must<Args1...>, Filter<Args2...>, ...>
template<class Model, class ...SpecificBooleanParams/*Must, Filter*/>
struct boolean {
    using value_type = ::model::Boolean<Model, typename std::decay_t<SpecificBooleanParams>::value_type...>;

    // Standalone itself serializer starts here:
    //    a) it must be capable to serialize model::Boolean by itseld
    template<class Parent>
    using serializer_parted_type =
                            QueryBooleanContextToJSONParted<Parent,
                                                            Model,
                                                            typename std::decay_t<SpecificBooleanParams>::value_type...>;
    //    b) serialize each subsequent subqueries: Must, Filter ... by using its hints as 'serializer_parted_type'
    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(QueryBooleanContextToJSON,
                                             serializer_parted_type<QueryBooleanContextToJSON>,
                                             typename SpecificBooleanParams::serializer_parted_type<QueryBooleanContextToJSON>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };
    // Standalone itself serializer ends here:
    using serializer_type = QueryBooleanContextToJSON;


    // serializer hint: how to serialize this underlying tag for final upperlevel tag serializer
    // starts here
    //      a) Must aggregate ALL types of subsequent serializer hints types through variadic pack
    //         combine all of them with own parted serializer hint for itself
    //         and combine all of it with future unknown final upper levels  serializers
    //         where `Parent` is final upper level serializer aggregator
    template<class ThisParent, template<typename> class ...QueryUpperLevels>
    using serializer_dispatcher_type  = txml::SerializerVirtualDispatcher<json::SerializerCore, QueryUpperLevels<ThisParent>..., serializer_parted_type<ThisParent>,
                                                                   typename SpecificBooleanParams::serializer_parted_type<ThisParent>...>;

    template<template<typename>class CustomSerializer, class ThisParent, template<typename> class ...QueryUpperLevels>
    using custom_serializer_dispatcher_type  = txml::SerializerVirtualDispatcher<json::SerializerCore, QueryUpperLevels<ThisParent>..., serializer_parted_type<ThisParent>,
                                                                   typename SpecificBooleanParams::custom_serializer_parted_type<ThisParent, CustomSerializer>...>;


    //      b) just syntax sugar to reduce template<template<template>> params
    //         because it do not know how to write it down just now...
    template<template<typename> class ...QueryUpperLevels>
    struct parent : public serializer_dispatcher_type<parent<QueryUpperLevels...>, QueryUpperLevels...>
    {
        using base_t = serializer_dispatcher_type<parent<QueryUpperLevels...>, QueryUpperLevels...>;

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
    ////////////

    template<class ...BooleanParamsTagsPack>
    boolean(BooleanParamsTagsPack &&...args)
    {
        instance_ptr  = std::make_shared<value_type>();
        (instance_ptr->template set<typename std::decay_t<BooleanParamsTagsPack>::value_type>(
                                       args.instance_ptr), ...);
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
    template<class Model, class ...SpecificBooleanParams>
    boolean<Model, std::decay_t<SpecificBooleanParams>...> boolean_tag(SpecificBooleanParams &&...args)
    {
        return boolean<Model, std::decay_t<SpecificBooleanParams>...> (std::forward<SpecificBooleanParams>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_BOOLEAN_HPP
