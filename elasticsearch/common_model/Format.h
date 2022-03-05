#ifndef COMMON_MODEL_FORMAT_H
#define COMMON_MODEL_FORMAT_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace common_model
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
}
#endif // COMMON_MODEL_FORMAT_H
