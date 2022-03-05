#ifndef ANSWER_MODEL_OBJECT_SIZE_H
#define ANSWER_MODEL_OBJECT_SIZE_H

#include <txml/txml_fwd.h>

namespace model
{
class Size : public txml::XMLNodeLeaf<Size, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Size, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "size";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Size(unsigned int size) : base_t(size) {}
};
}
#endif // ANSWER_MODEL_OBJECT_SIZE_H
