#ifndef IMAGE_DATA_MODEL_DESERIALIZER_H
#define IMAGE_DATA_MODEL_DESERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/images/data_model/model.hpp"

namespace elasticsearch
{
namespace image
{
namespace model
{
using namespace json;
template<class ParentAggregator>
TXML_PREPARE_DESERIALIZER_DISPATCHABLE_CLASS(from_data, ParentAggregator, FromJSON,
                                             data, IMAGE_DATA_MODEL_ELEMENTS)
{
    TXML_DESERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;

    template<class Tracer>
    std::shared_ptr<elasticsearch::image::model::element::Resolution>
    deserialize_impl(txml::details::SchemaDTag<elasticsearch::image::model::element::Resolution>, Tracer tracer)
    {
        using Type = elasticsearch::image::model::element::Resolution;
        tracer.trace(__FUNCTION__, " - ", Type::class_name());

        auto& [begin_it, end_it] = this->get_shared_mediator_object()->top();
        if (!this->template check_leaf_node_param<Type>(begin_it, end_it,
                                         ::json::utils::type_to_json_type<std::string>(),
                                         tracer))
        {
            return {};
        }

        return std::make_shared<Type>((begin_it++).value().template get<std::string>());
    }

    template<class Tracer>
    std::shared_ptr<elasticsearch::image::model::element::Location>
    deserialize_impl(txml::details::SchemaDTag<elasticsearch::image::model::element::Location>, Tracer tracer)
    {
        using Type = elasticsearch::image::model::element::Location;
        tracer.trace(__FUNCTION__, " - ", Type::class_name());

        auto& [begin_it, end_it] = this->get_shared_mediator_object()->top();
        if (!this->template check_leaf_node_param<Type>(begin_it, end_it,
                                         ::json::utils::type_to_json_type<std::string>(),
                                         tracer))
        {
            return {};
        }

        return std::make_shared<Type>((begin_it++).value().template get<std::string>());
    }
};
} // namespace model
} // namespace image
} // namespace elasticsearch
#endif // IMAGE_DATA_MODEL_DESERIALIZER_H
