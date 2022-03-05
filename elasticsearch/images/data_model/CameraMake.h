#ifndef ES_IMAGES_MODEL_CAMERA_MAKE_H
#define ES_IMAGES_MODEL_CAMERA_MAKE_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
class Camera: public txml::XMLNodeLeaf<Camera, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Camera, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "camera";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Camera(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Camera(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // ES_IMAGES_MODEL_CAMERA_MAKE_H
