#ifndef BOOK_DATA_MODEL_SCHEMA_SERIALIZER_H
#define BOOK_DATA_MODEL_SCHEMA_SERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/common_model/serializers/decorator.hpp"
namespace elasticsearch
{
namespace book
{
namespace model
{
using namespace json;
template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(to_schema, Parent, SchemaToJSON,
                                                  data, BOOK_DATA_MODEL_ELEMENTS)
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
        //this->json_object_stack_helper->push(nlohmann::json::object({{SerializedItem::class_name(),{{"type", "text"}}}}));
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
        //this->json_object_stack_helper->push(std::move(obj));
        /*this->json_object_stack_helper->push(nlohmann::json::object({
                                {SerializedItem::class_name(),{{"type", "text"}}},
                                elasticsearch::common_model::decorator::make_keyword()}));*/
    }
};
} // namespace model
} // namespace book
} // namespace elasticsearch
#endif // BOOK_DATA_MODEL_SCHEMA_SERIALIZER_H
