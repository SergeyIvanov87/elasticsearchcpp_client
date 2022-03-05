#ifndef ANSWER_MODEL_OBJECT_INDEX_H
#define ANSWER_MODEL_OBJECT_INDEX_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Index : public txml::XMLNodeLeaf<Index, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Index, std::string>;

    static constexpr std::string_view class_name()
    {
        return "index";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Index(std::string&& str) :
        base_t(std::move(str))
    {
    }
};
}
#endif // ANSWER_MODEL_OBJECT_INDEX_H
