#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_OBJECT_BOOLEAN_SERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_OBJECT_BOOLEAN_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/models.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/Bool.h"

namespace elasticsearch
{
namespace v7
{
namespace search
{
using namespace json;

template<class Model, class ...SpecificQueryParams>
struct QueryMustContextToJSON : public ToJSON<QueryMustContextToJSON<Model, SpecificQueryParams...>,
                                                ::model::Must<Model, SpecificQueryParams...>,
                                                    ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::must::Term<Model, SpecificQueryParams>...,
                                                        ::model::must::Terms<Model, SpecificQueryParams>...,
                                                        ::model::must::ElementToQuery<Model, SpecificQueryParams>...,
                                                SpecificQueryParams...>
{
    using base_t = ToJSON<QueryMustContextToJSON<Model, SpecificQueryParams...>,
                                ::model::Must<Model, SpecificQueryParams...>,
                                        ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                ::model::must::Term<Model, SpecificQueryParams>...,
                                                ::model::must::Terms<Model, SpecificQueryParams>...,
                                                ::model::must::ElementToQuery<Model, SpecificQueryParams>...,
                                            SpecificQueryParams...>;
    using base_t::base_t;
    using base_t::serialize_impl;

    template<class Tracer>
    void serialize_impl(const ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe SpecificQueryArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};


template<class Parent, class Model, class ...SpecificQueryParams>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(QueryMustContextToJSONParted, Parent, ToJSON,
                                                ::model::Must<Model, SpecificQueryParams...>,
                                                    ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::must::Term<Model, SpecificQueryParams>...,
                                                        ::model::must::Terms<Model, SpecificQueryParams>...,
                                                        ::model::must::ElementToQuery<Model, SpecificQueryParams>...,
                                                SpecificQueryParams...)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_impl(const ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe SpecificQueryArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};

template<class Parent, class Model, class ...SpecificQueryParams>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(MagicQueryMustContextToJSONParted, Parent, ToJSON,
                                                ::model::Must<Model, SpecificQueryParams...>,
                                                    ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::must::Term<Model, SpecificQueryParams>...,
                                                        ::model::must::Terms<Model, SpecificQueryParams>.../*,
                                                        ::model::must::ElementToQuery<Model, SpecificQueryParams>...*/)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_impl(const ::model::must::SpecificQueryArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe SpecificQueryArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};

template<class Model, class ...SpecificQueryParams>
struct QueryFilterContextToJSON : public ToJSON<QueryFilterContextToJSON<Model, SpecificQueryParams...>,
                                                ::model::Filter<Model, SpecificQueryParams...>,
                                                    ::model::filter::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::filter::Term<Model, SpecificQueryParams>...,
                                                        //::model::filter::Terms<Model, SpecificQueryParams>...,
                                                        ::model::filter::ElementToQuery<Model, SpecificQueryParams>...,
                                                SpecificQueryParams...>
{
    using base_t = ToJSON<QueryFilterContextToJSON<Model, SpecificQueryParams...>,
                                ::model::Filter<Model, SpecificQueryParams...>,
                                        ::model::filter::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                ::model::filter::Term<Model, SpecificQueryParams>...,
                                                //::model::filter::Terms<Model, SpecificQueryParams>...,
                                                ::model::filter::ElementToQuery<Model, SpecificQueryParams>...,
                                            SpecificQueryParams...>;
    using base_t::base_t;
    using base_t::serialize_impl;

    template<class Tracer>
    void serialize_impl(const ::model::filter::SpecificQueryArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe SpecificQueryArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};

template<class Parent, class Model, class ...SpecificQueryParams>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(QueryFilterContextToJSONParted, Parent, ToJSON,
                                                ::model::Filter<Model, SpecificQueryParams...>,
                                                    ::model::filter::SpecificQueryArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::filter::Term<Model, SpecificQueryParams>...,
                                                  //      ::model::filter::Terms<Model, SpecificQueryParams>...,
                                                        ::model::filter::ElementToQuery<Model, SpecificQueryParams>...,
                                                SpecificQueryParams...)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_impl(const ::model::filter::SpecificQueryArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe SpecificQueryArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};


template<class Parent, class Model, class ...SubContexts>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(QueryBooleanContextToJSONParted, Parent, ToJSON,
                                                ::model::Boolean<Model, SubContexts...>)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
};
}
}
}

#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_OBJECT_BOOLEAN_SERIALIZER_HPP
