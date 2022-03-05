#ifndef ANSWER_MODEL_OBJECT_ID_H
#define ANSWER_MODEL_OBJECT_ID_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Id : public txml::XMLNodeLeaf<Id, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Id, std::string>;

    static constexpr std::string_view class_name()
    {
        return "id";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Id(std::string&& str);
};
}
#endif // ANSWER_MODEL_OBJECT_ID_H
