#ifndef ANSWER_MODEL_SEARCH_OBJECT_PIT_ID_H
#define ANSWER_MODEL_SEARCH_OBJECT_PIT_ID_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class PitID : public txml::XMLNodeLeaf<PitID, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<PitID, std::string>;

    static constexpr std::string_view class_name()
    {
        return "pit_id";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    PitID(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_PIT_ID_H
