#ifndef ANSWER_MODEL_SEARCH_OBJECT_HITS_NODE_H
#define ANSWER_MODEL_SEARCH_OBJECT_HITS_NODE_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/search/object/resp/MaxScore.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/HitsTotal.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/HitsArray.h"

namespace model
{
template<class SpecificModel>
class HitsNode : public txml::XMLNode<HitsNode<SpecificModel>, HitsTotal, MaxScore, HitsArray<SpecificModel>>
{
public:
    using base_t = txml::XMLNode<HitsNode<SpecificModel>, HitsTotal, MaxScore, HitsArray<SpecificModel>>;

    static constexpr std::string_view class_name()
    {
        return "hits";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_HITS_NODE_H
