#ifndef IMAGE_DATA_MODEL_MODEL_H
#define IMAGE_DATA_MODEL_MODEL_H

#include <txml/txml_fwd.h>
#include "elasticsearch/common_model/model.hpp"
#include "elasticsearch/images/data_model/CameraMake.h"
#include "elasticsearch/images/data_model/CameraModel.h"
#include "elasticsearch/images/data_model/DigitizeTime.h"
#include "elasticsearch/images/data_model/GeoPoint.h"
#include "elasticsearch/images/data_model/OriginalTime.h"
#include "elasticsearch/images/data_model/Resolution.h"
#include "elasticsearch/images/data_model/Title.h"

namespace elasticsearch
{
namespace image
{
namespace model
{
#define IMAGE_DATA_MODEL_ELEMENTS       elasticsearch::image::model::element::Camera,              \
                                        elasticsearch::image::model::element::CameraModel,         \
                                        elasticsearch::image::model::element::DigitizeTime,        \
                                        elasticsearch::image::model::element::Location,            \
                                        elasticsearch::image::model::element::OriginalTime,        \
                                        elasticsearch::image::model::element::Resolution,          \
                                        elasticsearch::image::model::element::Title


struct data : public txml::XMLNode<data,
                                   IMAGE_DATA_MODEL_ELEMENTS,
                                   COMMON_DATA_MODEL_ELEMENTS>
{
    using base = txml::XMLNode<data,
                               IMAGE_DATA_MODEL_ELEMENTS,
                               COMMON_DATA_MODEL_ELEMENTS>;

    static constexpr std::string_view class_name()
    {
        return "ImageModel";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
}
}
#endif // IMAGE_DATA_MODEL_MODEL_H
