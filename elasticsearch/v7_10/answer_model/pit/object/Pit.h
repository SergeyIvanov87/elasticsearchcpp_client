#ifndef ANSWER_MODEL_OBJECT_PIT_H
#define ANSWER_MODEL_OBJECT_PIT_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/object/Id.h"
#include "elasticsearch/v7_10/answer_model/object/KeepAlive.h"

namespace model
{
class PIT: public txml::XMLNode<PIT, Id, KeepAlive>
{
public:
    using base_t = txml::XMLNode<PIT, Id, KeepAlive>;

    static constexpr std::string_view class_name()
    {
        return "pit";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_OBJECT_PIT_H
