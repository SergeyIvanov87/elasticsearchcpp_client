#ifndef ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_MATCH_H
#define ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_MATCH_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/search/object/full_text/Message.hpp"

namespace model
{
class Match: public txml::XMLNode<Match, Message>
{
public:
    using base_t = txml::XMLNode<Match, Message>;

    static constexpr std::string_view class_name()
    {
        return "match";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_FULL_TEXT_MATCH_H
