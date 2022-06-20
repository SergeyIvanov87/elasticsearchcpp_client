#ifndef ELASTIC_SEARCH_IMAGE_PACKER_INTERFACE_HPP
#define ELASTIC_SEARCH_IMAGE_PACKER_INTERFACE_HPP

#include <filesystem>
#include <memory>

#include <nlohmann/json.hpp>

#include "elasticsearch/interfaces/packer.hpp"
#include "elasticsearch/images/data_model/model.hpp"

namespace elasticsearch
{
namespace image
{
template <class Tracer>
struct packer_interface : virtual public elasticsearch::packer_interface
{
    virtual ~packer_interface() = default;
    virtual std::optional<elasticsearch::image::model::data> to_model(Tracer tracer = Tracer()) const = 0;
    virtual nlohmann::json to_json(Tracer tracer = Tracer()) const = 0;
};

struct packer_interface_aggregator : public elasticsearch::image::packer_interface<txml::EmptyTracer>,
                                     public elasticsearch::image::packer_interface<txml::StdoutTracer>
{};
}
}
#endif // ELASTIC_SEARCH_IMAGE_PACKER_INTERFACE_HPP
