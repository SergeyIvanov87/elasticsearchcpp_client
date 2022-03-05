#ifndef ANSWER_MODEL_SEARCH_OBJECT_TOOK_H
#define ANSWER_MODEL_SEARCH_OBJECT_TOOK_H

#include <txml/txml_fwd.h>

namespace model
{
class Took : public txml::XMLNodeLeaf<Took, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<Took, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "took";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Took(unsigned int size) : base_t(size) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_TOOK_H
