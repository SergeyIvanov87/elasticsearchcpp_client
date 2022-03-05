
#ifndef ANSWER_MODEL_OBJECT_RESOURCE_ID_H
#define ANSWER_MODEL_OBJECT_RESOURCE_ID_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class ResourceId : public txml::XMLNodeLeaf<ResourceId, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<ResourceId, std::string>;

    static constexpr std::string_view class_name()
    {
        return "resource.id";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ResourceId(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_RESOURCE_ID_H
