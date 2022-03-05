#ifndef ANSWER_MODEL_OBJECT_INDEX_UUID_H
#define ANSWER_MODEL_OBJECT_INDEX_UUID_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class IndexUUID : public txml::XMLNodeLeaf<IndexUUID, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<IndexUUID, std::string>;

    static constexpr std::string_view class_name()
    {
        return "index_uuid";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    IndexUUID(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_INDEX_UUID_H
