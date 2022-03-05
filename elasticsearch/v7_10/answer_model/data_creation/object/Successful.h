#ifndef ANSWER_MODEL_OBJECT_SUCCESSFUL_H
#define ANSWER_MODEL_OBJECT_SUCCESSFUL_H

#include <txml/txml_fwd.h>

namespace model
{
class Successful : public txml::XMLNodeLeaf<Successful, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Successful, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "successful";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Successful(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_SUCCESSFUL_H
