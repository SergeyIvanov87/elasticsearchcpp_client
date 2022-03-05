
#ifndef ANSWER_MODEL_SEARCH_OBJECT_RESP_SCORE_H
#define ANSWER_MODEL_SEARCH_OBJECT_RESP_SCORE_H

#include <txml/txml_fwd.h>

namespace model
{
class _Score : public txml::XMLNodeLeaf<_Score, float>
{
public:
    using base_t = txml::XMLNodeLeaf<_Score, float>;

    static constexpr std::string_view class_name()
    {
        return "_score";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _Score(float val) : base_t(val) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_RESP_SCORE_H
