#ifndef ANSWER_MODEL_OBJECT_DASH_VERSION_H
#define ANSWER_MODEL_OBJECT_DASH_VERSION_H

#include <txml/txml_fwd.h>

namespace model
{
class _Version : public txml::XMLNodeLeaf<_Version, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<_Version, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "_version";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _Version(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_VERSION_H
