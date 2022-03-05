#ifndef ES_IMAGES_MODEL_ORIGINAL_TIME_H
#define ES_IMAGES_MODEL_ORIGINAL_TIME_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
class OriginalTime: public txml::XMLNodeLeaf<OriginalTime, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<OriginalTime, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "original_time";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    OriginalTime(value_t&& value) :
        base_t(std::move(value))
    {
    }

    OriginalTime(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // ES_IMAGES_MODEL_ORIGINAL_TIME_H
