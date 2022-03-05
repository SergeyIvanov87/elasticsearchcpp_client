#ifndef ANSWER_MODEL_OBJECT_FOUND_H
#define ANSWER_MODEL_OBJECT_FOUND_H

#include <txml/txml_fwd.h>

namespace model
{
class Found : public txml::XMLNodeLeaf<Found, bool>
{
public:
    using base_t = txml::XMLNodeLeaf<Found, bool>;

    static constexpr std::string_view class_name()
    {
        return "found";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Found(bool code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_FOUND_H
