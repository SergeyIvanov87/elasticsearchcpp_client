#ifndef ES_IMAGES_MODEL_CAMERA_MODEL_H
#define ES_IMAGES_MODEL_CAMERA_MODEL_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
class CameraModel: public txml::XMLNodeLeaf<CameraModel, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<CameraModel, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "camera_model";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    CameraModel(value_t&& value) :
        base_t(std::move(value))
    {
    }

    CameraModel(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // ES_IMAGES_MODEL_CAMERA_MODEL_H
