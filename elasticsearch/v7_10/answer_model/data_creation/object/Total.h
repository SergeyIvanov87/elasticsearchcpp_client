#ifndef ANSWER_MODEL_OBJECT_TOTAL_H
#define ANSWER_MODEL_OBJECT_TOTAL_H

#include <txml/txml_fwd.h>

namespace model
{
class Total : public txml::XMLNodeLeaf<Total, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Total, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "total";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Total(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_TOTAL_H
