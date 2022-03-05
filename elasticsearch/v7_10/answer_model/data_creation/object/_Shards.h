#ifndef ANSWER_MODEL_OBJECT_DASH_SHARDS_H
#define ANSWER_MODEL_OBJECT_DASH_SHARDS_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/data_creation/object/Total.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/Successful.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/Failed.h"

namespace model
{
class _Shards: public txml::XMLNode<_Shards, Total, Successful, Failed>

{
public:
    using base_t = txml::XMLNode<_Shards, Total, Successful, Failed>;

    static constexpr std::string_view class_name()
    {
        return "_shards";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_SHARDS_H
