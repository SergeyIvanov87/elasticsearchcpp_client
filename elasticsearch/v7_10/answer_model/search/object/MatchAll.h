#ifndef ANSWER_MODEL_SEARCH_OBJECT_MATCH_ALL_H
#define ANSWER_MODEL_SEARCH_OBJECT_MATCH_ALL_H

#include <txml/txml_fwd.h>

namespace model
{
class EmptyModel {};
struct EmptyParam {using value_type = EmptyParam; };

class MatchAll : public txml::XMLNode<MatchAll>
{
public:
    using base_t = txml::XMLNode<MatchAll>;

    static constexpr std::string_view class_name()
    {
        return "match_all";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_MATCH_ALL_H
