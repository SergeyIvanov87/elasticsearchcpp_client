#ifndef ANSWER_MODEL_OBJECT_RESOURCE_TYPE_H
#define ANSWER_MODEL_OBJECT_RESOURCE_TYPE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class ResourceType : public txml::XMLNodeLeaf<ResourceType, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<ResourceType, std::string>;

    static constexpr std::string_view class_name()
    {
        return "resource.type";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ResourceType(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_RESOURCE_TYPE_H
