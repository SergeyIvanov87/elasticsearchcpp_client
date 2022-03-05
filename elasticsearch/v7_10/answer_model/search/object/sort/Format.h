#ifndef ANSWER_MODEL_SEARCH_OBJECT_SORT_FORMAT_H
#define ANSWER_MODEL_SEARCH_OBJECT_SORT_FORMAT_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Format: public txml::XMLNodeLeaf<Format, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Format, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "format";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Format(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Format(const value_t& value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_SORT_FORMAT_H
