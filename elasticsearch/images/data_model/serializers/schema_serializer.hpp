#ifndef IMAGE_DATA_MODEL_SCHEMA_SERIALIZER_H
#define IMAGE_DATA_MODEL_SCHEMA_SERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/common_model/serializers/decorator.hpp"

namespace elasticsearch
{
namespace image
{
namespace model
{
using namespace json;
template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(to_schema, Parent, SchemaToJSON,
                                                  data, IMAGE_DATA_MODEL_ELEMENTS)
{
    TXML_SCHEMA_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<element::Location>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{element::Location::class_name().data(),{{"type", "geo_point"}}}}));
    }

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<element::OriginalTime>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object(
            {{element::OriginalTime::class_name().data(),
             {{"type", "date"},{"format", "yyyy:MM:dd HH:mm:ss||yyyy-MM-dd||epoch_millis"}}}}));
    }

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<element::DigitizeTime>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object(
            {{elasticsearch::image::model::element::DigitizeTime::class_name().data(),
             {{"type", "date"},{"format", "yyyy:MM:dd HH:mm:ss||yyyy-MM-dd||epoch_millis"}}}}));
    }

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
    void serialize_schema_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {/*
        this->json_object_stack_helper->push(nlohmann::json::object({
                                {SerializedItem::class_name().data(),{
                                        {"type", "text"},
                                        {elasticsearch::common_model::decorator::make_keyword()}}
                                }
        }));*/
        this->json_object_stack_helper->push(nlohmann::json::object(
        {
            {SerializedItem::class_name().data(),{
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
};
} // namespace model
} // namespace image
} // namespace elasticsearch
#endif // IMAGE_DATA_MODEL_SCHEMA_SERIALIZER_H
