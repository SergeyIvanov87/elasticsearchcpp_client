#ifndef ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_MESSAGE_H
#define ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_MESSAGE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Message : public txml::XMLNodeLeaf<Message, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Message, std::string>;
    using base_t::base_t;
    static constexpr std::string_view class_name()
    {
        return "message";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_MESSAGE_H
