#ifndef ANSWER_MODEL_OBJECT_DASH_INDEX_H
#define ANSWER_MODEL_OBJECT_DASH_INDEX_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class _Index : public txml::XMLNodeLeaf<_Index, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<_Index, std::string>;

    static constexpr std::string_view class_name()
    {
        return "_index";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _Index(std::string&& str) :
        base_t(std::move(str))
    {
    }
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_INDEX_H
