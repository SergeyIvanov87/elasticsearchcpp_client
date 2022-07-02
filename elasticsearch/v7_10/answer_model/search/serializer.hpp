#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
using namespace json;

TXML_DECLARE_SERIALIZER_CLASS(CommonSubrequestToJSON, ToJSON,
                                                      common_subrequest,
                                                      ::model::PIT,
                                                        ::model::Id,
                                                        ::model::KeepAlive,
                                                    ::model::Size)
{
    TXML_SERIALIZER_OBJECT

    template<class Tracer>
    void serialize_impl(const common_subrequest &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe common_subrequest by itself");

        val.template make_format_serialize(*this, tracer);
    }
};

template<class Model, class ...SpecificSortParams>
struct SortSubrequestToJSON : public ToJSON<SortSubrequestToJSON<Model, SpecificSortParams...>,
                                            sort_subrequest<Model, SpecificSortParams...>,
                                                ::model::SortArray<Model, SpecificSortParams...>,
                                                    ::model::SortArrayElement<Model, SpecificSortParams...>,
                                                        ::model::SortRecord<Model, SpecificSortParams>...,
                                                            ::model::UnmappedType,
                                                            ::model::Order,
                                                            ::model::Format>
{
    using base_t = ToJSON<SortSubrequestToJSON<Model, SpecificSortParams...>,
                                            sort_subrequest<Model, SpecificSortParams...>,
                                                ::model::SortArray<Model, SpecificSortParams...>,
                                                    ::model::SortArrayElement<Model, SpecificSortParams...>,
                                                        ::model::SortRecord<Model, SpecificSortParams>...,
                                                            ::model::UnmappedType,
                                                            ::model::Order,
                                                            ::model::Format>;
    using base_t::base_t;
    using base_t::serialize_impl;

    template<class Tracer>
    void serialize_impl(const sort_subrequest<Model, SpecificSortParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe sort_subrequest by itself");

        val.template make_format_serialize(*this, tracer);
    }

    template<class Tracer>
    void serialize_impl(const ::model::SortArrayElement<Model, SpecificSortParams...> &val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe SortArrayElement by itself");
        val.template make_format_serialize(*this, tracer);
    }
};

template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(MatchAllToJSON, Parent,
                                                  ToJSON,
                                                            ::model::MatchAll)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;

    template<class Tracer>
    void serialize_impl(const ::model::MatchAll&)
    {
        auto mediator = this->get_shared_mediator_object();
        json element({{::model::MatchAll::class_name(), ""}});
        mediator->push(std::move(element));
    }
};
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_DESERIALIZER_HPP
