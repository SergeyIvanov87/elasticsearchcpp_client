#ifndef ANSWER_MODEL_OBJECT_NUMFREED_H
#define ANSWER_MODEL_OBJECT_NUMFREED_H

#include <txml/txml_fwd.h>

namespace model
{
class NumFreed : public txml::XMLNodeLeaf<NumFreed, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<NumFreed, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "num_freed";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    NumFreed(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_NUMFREED_H
