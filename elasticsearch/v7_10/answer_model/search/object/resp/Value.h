#ifndef ANSWER_MODEL_SEARCH_OBJECT_VALUE_H
#define ANSWER_MODEL_SEARCH_OBJECT_VALUE_H

#include <txml/txml_fwd.h>

namespace model
{
class Value : public txml::XMLNodeLeaf<Value, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Value, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "value";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Value(unsigned int size) : base_t(size) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_VALUE_H
