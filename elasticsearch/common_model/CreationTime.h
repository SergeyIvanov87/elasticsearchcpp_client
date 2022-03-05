#ifndef COMMON_MODEL_CREATION_DATETIME_H
#define COMMON_MODEL_CREATION_DATETIME_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace common_model
{
class CreationDateTime: public txml::XMLNodeLeaf<CreationDateTime, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<CreationDateTime, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "creation_datetime";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    CreationDateTime(value_t&& value) :
        base_t(std::move(value))
    {
    }

    CreationDateTime(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
#endif // COMMON_MODEL_CREATION_DATETIME_H
