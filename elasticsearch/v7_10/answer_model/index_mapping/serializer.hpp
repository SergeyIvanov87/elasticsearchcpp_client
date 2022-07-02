#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/index_mapping/index_mapping_models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping
{
using namespace json;

template<class SpecificModel, class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(RequestToJSON, Parent,
                                                  SchemaToJSON,
                                                            request<SpecificModel>,
                                                            ::model::Mappings<SpecificModel>,
                                                            ::model::Properties<SpecificModel>)
{
    TXML_SCHEMA_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<request<SpecificModel>>, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - skipe request by itself");

        request<SpecificModel>::template make_schema_serialize(
                *this, tracer);
    }
};




template<class SpecificModel, template<typename> class ...SpecificModelSerializer>
struct RequestSerializer : public txml::SerializerSchemaDispatcher<json::SerializerCore,
                                                                   RequestToJSON<SpecificModel,
                                                                                 RequestSerializer<SpecificModel,
                                                                                                   SpecificModelSerializer...>>,
                                                                   SpecificModelSerializer<RequestSerializer<SpecificModel,
                                                                                                             SpecificModelSerializer...>>...>
{
    using base_t = txml::SerializerSchemaDispatcher<json::SerializerCore,
                                                    RequestToJSON<SpecificModel,
                                                                  RequestSerializer<SpecificModel,
                                                                                    SpecificModelSerializer...>>,
                                                    SpecificModelSerializer<RequestSerializer<SpecificModel,
                                                                              SpecificModelSerializer...>>...>;
    static constexpr const char* name() { return "RequestSerializer"; }
    using json = nlohmann::json;
    RequestSerializer(std::shared_ptr<std::stack<json>> shared_object_stack =
                                     std::shared_ptr<std::stack<json>>(new std::stack<json>)) :
        SerializerCore(shared_object_stack),
        base_t(shared_object_stack)
    {
    }
};
} // namespace index_mapping
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_DESERIALIZER_HPP
