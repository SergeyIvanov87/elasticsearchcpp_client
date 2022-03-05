#ifndef COMMON_MODEL_SOURCE_NAME_H
#define COMMON_MODEL_SOURCE_NAME_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace common_model
{
class SourceName: public txml::XMLNodeLeaf<SourceName, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<SourceName, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "source_name";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    SourceName(value_t&& value) :
        base_t(std::move(value))
    {
    }

    SourceName(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
#endif // COMMON_MODEL_SOURCE_NAME_H
