#ifndef COMMON_MODEL_DESCRIPTION_H
#define COMMON_MODEL_DESCRIPTION_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace common_model
{
class Description: public txml::XMLNodeLeaf<Description, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Description, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "description";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Description(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Description(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
#endif // COMMON_MODEL_DESCRIPTION_H
