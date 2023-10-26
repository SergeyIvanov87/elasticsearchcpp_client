#ifndef ELASTICSEARCH_AUX_DOC_INDEX_MODEL_SCHEMA_SERIALIZER_HPP
#define ELASTICSEARCH_AUX_DOC_INDEX_MODEL_SCHEMA_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/service/doc_id/model.hpp"

namespace elasticsearch
{
namespace aux
{
namespace model
{
using namespace json;
template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(to_schema, Parent, SchemaToJSON,
                                                  AUX_DATA_MODEL_ELEMENTS)
{
    TXML_SCHEMA_SERIALIZER_DISPATCHABLE_OBJECT

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::ContainerTag&& t, Tracer &tracer)
    {
        base_t::template serialize_schema_tag_impl<SerializedItem>(t, tracer);
        nlohmann::json cur_json_element = nlohmann::json::object({{"properties",
                                                this->json_object_stack_helper->top()[SerializedItem::class_name().data()]}});
        this->json_object_stack_helper->pop();
        this->json_object_stack_helper->push({{SerializedItem::class_name().data(), std::move(cur_json_element)}});
    }

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::LeafTag&&, Tracer &)
    {
        this->json_object_stack_helper->push(nlohmann::json::object(
        {
            {SerializedItem::class_name().data(),{
                {"type", "text"}
            }},
        })
        );
    }
};
} // namespace model
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_AUX_DOC_INDEX_MODEL_SCHEMA_SERIALIZER_HPP
