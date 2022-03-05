
#ifndef ANSWER_MODEL_SEARCH_OBJECT_MAX_SCORE_H
#define ANSWER_MODEL_SEARCH_OBJECT_MAX_SCORE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class MaxScore : public txml::XMLNodeLeaf<MaxScore, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<MaxScore, std::string>;

    static constexpr std::string_view class_name()
    {
        return "max_score";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    MaxScore(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_MAX_SCORE_H
