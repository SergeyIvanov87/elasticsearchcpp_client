#ifndef COMMON_DATA_MODEL_SCHEMA_SERIALIZER_H
#define COMMON_DATA_MODEL_SCHEMA_SERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/common_model/model.hpp"
#include "elasticsearch/common_model/serializers/decorator.hpp"

namespace elasticsearch
{
namespace common_model
{
using namespace json;
template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(to_schema, Parent, SchemaToJSON,
                                                  COMMON_DATA_MODEL_ELEMENTS)
{
    TXML_SCHEMA_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<BinaryBlob>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{BinaryBlob::class_name(),{{"type", "binary"}}}}));
    }

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<CreationDateTime>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{CreationDateTime::class_name(),{{"type", "date"}}}}));
    }

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<Tags>, Tracer tracer)
    {
        //this->json_object_stack_helper->push(nlohmann::json::object({{Tags::class_name(),{{"type", "text"}}}}));
        this->json_object_stack_helper->push(nlohmann::json::object(
        {
            {Tags::class_name(),{
                {"type", "text"},
                {"fields", {
                    {"keyword", {
                        {"type", "keyword"},
                    }},
                }},
            }},
        })
        );
    }

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<Preview>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{Preview::class_name(),{{"type", "binary"}}}}));
    }

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::ContainerTag&& t, Tracer &tracer)
    {
        base_t::template serialize_schema_tag_impl<SerializedItem>(t, tracer);
        nlohmann::json cur_json_element = nlohmann::json::object({{"properties",
                                                this->json_object_stack_helper->top()[SerializedItem::class_name()]}});
        this->json_object_stack_helper->pop();
        this->json_object_stack_helper->push({{SerializedItem::class_name(), std::move(cur_json_element)}});
    }

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {
        //this->json_object_stack_helper->push(nlohmann::json::object({{SerializedItem::class_name(),{{"type", "text"}, decorator::make_keyword()}}}));
        this->json_object_stack_helper->push(nlohmann::json::object(
        {
            {SerializedItem::class_name(),{
                {"type", "text"},
                {"fields", {
                    {"keyword", {
                        {"type", "keyword"},
                    }},
                }},
            }},
        })
        );
    }

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<OriginalPath>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object(
        {
            {OriginalPath::class_name(),{
                {"type", "keyword"},    //search by full path
                {"fields", {
                    {"keyword", {
                        {"type", "keyword"},
                    }},
                }},
            }},
        })
        );
    }
};
} // namespace common_model
} // namespace elasticsearch
#endif // COMMON_DATA_MODEL_SCHEMA_SERIALIZER_H
