#ifndef ANSWER_MODEL_OBJECT_STATUS_H
#define ANSWER_MODEL_OBJECT_STATUS_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Status : public txml::XMLNodeLeaf<Status, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Status, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "status";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Status(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_STATUS_H
