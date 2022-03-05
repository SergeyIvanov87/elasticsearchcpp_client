#if 0
//7.17!!!

#ifndef ANSWER_MODEL_SEARCH_OBJECT_SORT_SHARD_DOC_H
#define ANSWER_MODEL_SEARCH_OBJECT_SORT_SHARD_DOC_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class ShardDoc: public txml::XMLNodeLeaf<ShardDoc, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<ShardDoc, std::string>;
    using value_t = typename base_t::value_t;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "_shard_doc";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ShardDoc(value_t&& value) :
        base_t(std::move(value))
    {
    }

    ShardDoc(const value_t& value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_SORT_SHARD_DOC_H
#endif
