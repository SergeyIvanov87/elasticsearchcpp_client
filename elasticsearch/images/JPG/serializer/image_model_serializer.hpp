#ifndef ELASTICSEARCH_IMAGE_JPG_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_IMAGE_JPG_MODEL_SERIALIZER_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/images/JPG/model/ExifData.h"

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace jpg
{
struct to_model_data : public txml::FormatSerializerBase<to_model_data, txml::StaticCheckUnscopedElement,
                                                         JPG::ExifData, JPG_DATA_MODEL_ELEMENTS>
{
    std::optional<elasticsearch::image::model::data> data_model;

    template<class Tracer>
    void serialize_impl(const ::JPG::ExifData& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::JPG::ExifData::class_name());
        data_model = std::make_optional<elasticsearch::image::model::data>();
        val.make_format_serialize(*this, tracer);
        data_model->emplace<elasticsearch::common_model::Format>("jpg");
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::Camera& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::Camera::class_name());
        data_model->emplace<elasticsearch::image::model::element::Camera>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::CameraModel& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::CameraModel::class_name());
        data_model->emplace<elasticsearch::image::model::element::CameraModel>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::DigitizeTime& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::DigitizeTime::class_name());
        data_model->emplace<elasticsearch::image::model::element::DigitizeTime>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::OriginalTime& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::OriginalTime::class_name());
        data_model->emplace<elasticsearch::image::model::element::OriginalTime>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::Resolution& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::Resolution::class_name());
        data_model->emplace<elasticsearch::image::model::element::Resolution>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const JPG::Description& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", JPG::Description::class_name());
        data_model->emplace<elasticsearch::common_model::Description>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::Location& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::Location::class_name());
        data_model->emplace<elasticsearch::image::model::element::Location>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::Title& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::Title::class_name());
        data_model->emplace<elasticsearch::image::model::element::Title>(val.value());
    }
};
} // namespace jpg
} // namespace model
} // namespace image
} // namespace elasticsearch
#endif // ELASTICSEARCH_IMAGE_JPG_MODEL_SERIALIZER_HPP
