#ifndef ES_IMAGES_MODEL_RESOLUTION_H
#define ES_IMAGES_MODEL_RESOLUTION_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
class resolution
{
public:
    using value_t = int16_t;

    resolution(value_t w, value_t h);
    resolution(const std::string& from_string);
    value_t get_width() const;
    value_t get_height() const;
    std::string to_string() const;
private:
    value_t width;
    value_t height;
};
class Resolution: public txml::XMLNodeLeaf<Resolution, resolution>
{
public:
    using base_t = txml::XMLNodeLeaf<Resolution, resolution>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "resolution";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Resolution(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Resolution(resolution::value_t w, resolution::value_t h) :
        Resolution(value_t(w, h))
    {
    }

    Resolution(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // ES_IMAGES_MODEL_RESOLUTION_H
