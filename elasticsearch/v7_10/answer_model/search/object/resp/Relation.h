
#ifndef ANSWER_MODEL_SEARCH_OBJECT_RELATION_H
#define ANSWER_MODEL_SEARCH_OBJECT_RELATION_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Relation : public txml::XMLNodeLeaf<Relation, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Relation, std::string>;

    static constexpr std::string_view class_name()
    {
        return "relation";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Relation(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_RELATION_H
