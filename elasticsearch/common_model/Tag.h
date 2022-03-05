#ifndef COMMON_MODEL_TAGS_H
#define COMMON_MODEL_TAGS_H

#include <list>
#include <string>
#include <regex>

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace common_model
{
class Tags: public txml::XMLNodeLeaf<Tags, std::list<std::string>>
{
public:
    using base_t = txml::XMLNodeLeaf<Tags, std::list<std::string>>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "tags";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Tags(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Tags(const value_t& value) :
        base_t(value)
    {
    }

    Tags(const std::string &flat_list, const std::string &separators) :
        Tags (value_t{})
    {
        std::regex rcore(separators);
        getValue() = value_t{std::sregex_token_iterator(flat_list.begin(), flat_list.end(), rcore, -1), std::sregex_token_iterator()};
    }
};
}
}
#endif // COMMON_MODEL_TAGS_H
