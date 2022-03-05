#ifndef ANSWER_MODEL_OBJECT_ACKNOWLEDGED_H
#define ANSWER_MODEL_OBJECT_ACKNOWLEDGED_H

#include <iostream>

#include <txml/txml_fwd.h>

namespace model
{
class Ack : public txml::XMLNodeLeaf<Ack, bool>
{
public:
    using base_t = txml::XMLNodeLeaf<Ack, bool>;

    static constexpr std::string_view class_name()
    {
        return "acknowledged";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }


    Ack(bool value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_OBJECT_ACKNOWLEDGED_H
