#ifndef ANSWER_MODEL_OBJECT_TYPE_H
#define ANSWER_MODEL_OBJECT_TYPE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Type : public txml::XMLNodeLeaf<Type, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Type, std::string>;

    static constexpr std::string_view class_name()
    {
        return "type";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Type(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_TYPE_H
