#ifndef ANSWER_MODEL_OBJECT_SHARDS_ACK_H
#define ANSWER_MODEL_OBJECT_SHARDS_ACK_H

#include <iostream>

#include <txml/txml_fwd.h>

namespace model
{
class ShardsAck : public txml::XMLNodeLeaf<ShardsAck, bool>

{
public:
    using base_t = txml::XMLNodeLeaf<ShardsAck, bool>;

    static constexpr std::string_view class_name()
    {
        return "shards_acknowledged";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }


    ShardsAck(bool value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_OBJECT_SHARDS_ACK_H
