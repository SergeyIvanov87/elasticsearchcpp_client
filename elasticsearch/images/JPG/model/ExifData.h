#ifndef IMAGE_EXIF_JPG_DATA_H
#define IMAGE_EXIF_JPG_DATA_H

#include <txml/txml_fwd.h>

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/images/JPG/model/Description.h"

namespace JPG
{
#define JPG_DATA_MODEL_ELEMENTS         elasticsearch::image::model::element::Camera,              \
                                        JPG::Description,                                          \
                                        elasticsearch::image::model::element::CameraModel,         \
                                        elasticsearch::image::model::element::DigitizeTime,        \
                                        elasticsearch::image::model::element::Location,            \
                                        elasticsearch::image::model::element::OriginalTime,        \
                                        elasticsearch::image::model::element::Resolution,          \
                                        elasticsearch::image::model::element::Title

class ExifData: public txml::XMLNode<ExifData, JPG_DATA_MODEL_ELEMENTS>
{
public:
    using base_t = txml::XMLNode<ExifData, JPG_DATA_MODEL_ELEMENTS>;

    static constexpr std::string_view class_name()
    {
        return "exif_data";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace JPG
#endif // IMAGE_EXIF_JPG_DATA_H
