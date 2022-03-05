#ifndef ES_IMAGES_MODEL_DIGITIZE_TIME_H
#define ES_IMAGES_MODEL_DIGITIZE_TIME_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
class DigitizeTime: public txml::XMLNodeLeaf<DigitizeTime, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<DigitizeTime, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "digitize_time";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    DigitizeTime(value_t&& value) :
        base_t(std::move(value))
    {
    }

    DigitizeTime(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // ES_IMAGES_MODEL_DIGITIZE_TIME_H
