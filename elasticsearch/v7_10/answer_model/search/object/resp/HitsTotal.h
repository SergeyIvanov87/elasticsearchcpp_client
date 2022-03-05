#ifndef ANSWER_MODEL_SEARCH_OBJECT_TOTAL_H
#define ANSWER_MODEL_SEARCH_OBJECT_TOTAL_H

#include <txml/txml_fwd.h>

#include "elasticsearch/v7_10/answer_model/search/object/resp/Relation.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/Value.h"

namespace model
{
class HitsTotal : public txml::XMLNode<HitsTotal, Relation, Value>
{
public:
    using base_t = txml::XMLNode<HitsTotal, Relation, Value>;

    static constexpr std::string_view class_name()
    {
        return "total";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_TOTAL_H
