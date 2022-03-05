#ifndef ANSWER_MODEL_SEARCH_OBJECT_TIMEDOUT_H
#define ANSWER_MODEL_SEARCH_OBJECT_TIMEDOUT_H

#include <iostream>

#include <txml/txml_fwd.h>

namespace model
{
class TimedOut : public txml::XMLNodeLeaf<TimedOut, bool>
{
public:
    using base_t = txml::XMLNodeLeaf<TimedOut, bool>;

    static constexpr std::string_view class_name()
    {
        return "timed_out";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }


    TimedOut(bool value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_TIMEDOUT_H
