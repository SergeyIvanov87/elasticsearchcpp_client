#ifndef ANSWER_MODEL_OBJECT_DASH_TYPE_H
#define ANSWER_MODEL_OBJECT_DASH_TYPE_H

#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class _Type : public txml::XMLNodeLeaf<_Type, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<_Type, std::string>;

    static constexpr std::string_view class_name()
    {
        return "_type";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _Type(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_TYPE_H
