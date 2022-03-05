#ifndef ANSWER_MODEL_OBJECT_FAILED_H
#define ANSWER_MODEL_OBJECT_FAILED_H

#include <txml/txml_fwd.h>

namespace model
{
class Failed : public txml::XMLNodeLeaf<Failed, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Failed, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "failed";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Failed(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_FAILED_H
