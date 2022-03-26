#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_SERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/models.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/full_text/SimpleQueryString.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
using namespace json;

template<class Model, class ...SpecificQueryParams>
struct QuerySimpleQueryStringToJSON : public ToJSON<QuerySimpleQueryStringToJSON<Model, SpecificQueryParams...>,
                                                ::model::full_text::SimpleQueryString<Model, SpecificQueryParams...>,
                                                    ::model::full_text::Query,
                                                    ::model::full_text::FieldsArray<Model, SpecificQueryParams...>,
                                                        ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::full_text::Fields<Model, SpecificQueryParams>...>
{
    using base_t = ToJSON<QuerySimpleQueryStringToJSON<Model, SpecificQueryParams...>,
                                                ::model::full_text::SimpleQueryString<Model, SpecificQueryParams...>,
                                                    ::model::full_text::Query,
                                                    ::model::full_text::FieldsArray<Model, SpecificQueryParams...>,
                                                        ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::full_text::Fields<Model, SpecificQueryParams>...>;
    using base_t::base_t;
    using base_t::serialize_impl;

    template<class Tracer>
    void serialize_impl(const ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skip FieldsArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};


template<class Parent, class Model, class ...SpecificQueryParams>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(QuerySimpleQueryStringToJSONParted, Parent, ToJSON,
                                                ::model::full_text::SimpleQueryString<Model, SpecificQueryParams...>,
                                                    ::model::full_text::Query,
                                                    ::model::full_text::FieldsArray<Model, SpecificQueryParams...>,
                                                        ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::full_text::Fields<Model, SpecificQueryParams>...)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_impl(const ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe FieldsArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};

template<class Parent, class Model, class ...SpecificQueryParams>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(MagicQuerySimpleQueryStringContextToJSONParted, Parent, ToJSON,
                                                ::model::full_text::SimpleQueryString<Model, SpecificQueryParams...>,
                                                    ::model::full_text::Query,
                                                    ::model::full_text::FieldsArray<Model, SpecificQueryParams...>,
                                                        ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...>,
                                                        ::model::full_text::Fields<Model, SpecificQueryParams>...)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_impl(const ::model::full_text::FieldsArrayElement<Model, SpecificQueryParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe FieldsArrayElement by itself");
        val.template format_serialize_elements(*this, tracer);
    }
};
}
}
}

#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_SERIALIZER_HPP
