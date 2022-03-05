#ifndef ANSWER_MODEL_OBJECT_DASH_ID_H
#define ANSWER_MODEL_OBJECT_DASH_ID_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class _Id : public txml::XMLNodeLeaf<_Id, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<_Id, std::string>;

    static constexpr std::string_view class_name()
    {
        return "_id";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _Id(std::string&& str) :
        base_t(std::move(str))
    {
    }
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_ID_H
