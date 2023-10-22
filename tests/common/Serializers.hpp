#ifndef TESTS_COMMON_SERIALZIERS_HPP
#define TESTS_COMMON_SERIALZIERS_HPP

#include <nlohmann/json.hpp>
#include <txml/applications/json/json.hpp>
#include "tests/common/Nodes.hpp"

namespace tests
{
using namespace json;
template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(StubLeafNodeSerializer, Parent, SchemaToJSON,
                                                  StubLeafNode)
{
    TXML_SCHEMA_SERIALIZER_DISPATCHABLE_OBJECT

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{SerializedItem::class_name(),{{"type", "text"}}}}));
    }
};


TXML_DECLARE_SERIALIZER_CLASS(StubLeafNodeSerializerE2E, ToJSON,
                                                         StubLeafNode)
{
    TXML_SERIALIZER_OBJECT
    using json = nlohmann::json;
};

template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(StubLeafNodeSerializerMixin, Parent, ToJSON,
                                                         StubLeafNode)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;
};



template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(StubModelSerializer, Parent, SchemaToJSON,
                                                  StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string)
{
    TXML_SCHEMA_SERIALIZER_DISPATCHABLE_OBJECT

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{SerializedItem::class_name(),{{"type", "text"}}}}));
    }
};


TXML_DECLARE_SERIALIZER_CLASS(StubModelSerializerE2E, ToJSON,
                              StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string)
{
    TXML_SERIALIZER_OBJECT
    using json = nlohmann::json;
};


template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(CustomModelSerializer, Parent, SchemaToJSON,
                                                  CustomModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string, CustomNode)
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

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<CustomNode>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{CustomNode::class_name(),{{"type", "date"}}}}));
    }


    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {
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
/*
template<class Tracer>
void serialize_tag_impl(const ::model::must::ElementToQuery<tests::CustomModel, tests::CustomNode> &v, const txml::LeafTag&, Tracer& tracer)
{
}*/
/*    template<class SerializedItem, class Tracer>
    void serialize_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{SerializedItem::class_name(),{{"type", "custom_text"}}}}));
   }
*/
};
//*/

template<class Parent>
TXML_PREPARE_SCHEMA_SERIALIZER_DISPATCHABLE_CLASS(CustomModelToAddSerializer, Parent, SchemaToJSON,
                                                  CustomModelToAdd, CustomNodeToAdd)
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

    template<class Tracer>
    void serialize_schema_impl(txml::details::SchemaTag<CustomNodeToAdd>, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{CustomNodeToAdd::class_name(),{{"type", "date"}}}}));
    }

    template<class SerializedItem, class Tracer>
    void serialize_schema_tag_impl(txml::LeafTag&& t, Tracer &tracer)
    {
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
}
#endif // TESTS_COMMON_SERIALZIERS_HPP
