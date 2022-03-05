#ifndef IMAGE_DATA_MODEL_SERIALIZER_H
#define IMAGE_DATA_MODEL_SERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/images/data_model/model.hpp"

namespace elasticsearch
{
namespace image
{
using namespace json;
template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(to_data, Parent, ToJSON,
                                           model::data, IMAGE_DATA_MODEL_ELEMENTS)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::Resolution& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::Resolution::class_name().data());
        this->json_object_stack_helper->push(nlohmann::json::object(
                            {{elasticsearch::image::model::element::Resolution::class_name().data(),
                             val.getValue().to_string()}}));
    }

    template<class Tracer>
    void serialize_impl(const elasticsearch::image::model::element::Location& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", elasticsearch::image::model::element::Location::class_name().data());
        this->json_object_stack_helper->push(nlohmann::json::object(
                            {{elasticsearch::image::model::element::Location::class_name().data(),
                             val.getValue().to_string()}}));
    }
};
} // namespace image
} // namespace elasticsearch
#endif // IMAGE_DATA_MODEL_SERIALIZER_H
