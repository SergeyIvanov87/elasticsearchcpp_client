#ifndef ANSWER_MODEL_OBJECT_CAUSED_BY_H
#define ANSWER_MODEL_OBJECT_CAUSED_BY_H

#include <txml/txml_fwd.h>

#include "elasticsearch/v7_10/answer_model/object/Type.h"
#include "elasticsearch/v7_10/answer_model/object/Reason.h"

namespace model
{
class CausedBy: public txml::XMLNode<CausedBy, Type, Reason>
{
public:
    using base_t = txml::XMLNode<CausedBy, Type, Reason>;

    static constexpr std::string_view class_name()
    {
        return "caused_by";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_OBJECT_CAUSED_BY_H
