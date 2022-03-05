#ifndef ANSWER_MODEL_OBJECT_SUCCEDED_H
#define ANSWER_MODEL_OBJECT_SUCCEDED_H

#include <txml/txml_fwd.h>

namespace model
{
class Succeeded : public txml::XMLNodeLeaf<Succeeded, bool>
{
public:
    using base_t = txml::XMLNodeLeaf<Succeeded, bool>;

    static constexpr std::string_view class_name()
    {
        return "succeeded";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Succeeded(bool result) : base_t(result) {}
};
}
#endif // ANSWER_MODEL_OBJECT_SUCCEDED_H
