#ifndef ELASTICSEARCH_IMAGES_REQUEST_GET_IMAGE_HPP
#define ELASTICSEARCH_IMAGES_REQUEST_GET_IMAGE_HPP

#include "elasticsearch/v7_10/request/get_data.hpp"
#include "elasticsearch/images/data_model/model.hpp"

// forward declarations
class Connection;

namespace elasticsearch
{
namespace image
{
namespace get
{
class transaction : public elasticsearch::v7::get_json_data::transaction
{
public:
    using impl_t = elasticsearch::v7::get_json_data::transaction;
    using receiver = impl_t::receiver;
    using response = elasticsearch::image::model::data;

    transaction(const std::string& host);
    ~transaction();

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<response> get_response(Tracer tracer = Tracer()) const;
};
} // namespace get
} // namespace image
} // namespace elasticsearch
#endif // ELASTICSEARCH_IMAGES_REQUEST_GET_IMAGE_HPP
