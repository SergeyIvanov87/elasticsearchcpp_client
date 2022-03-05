#ifndef ELASTICSEARCH_BOOKS_REQUEST_DELETE_IMAGE_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_DELETE_IMAGE_HPP

#include "elasticsearch/v7_10/request/delete_data.hpp"
namespace elasticsearch
{
namespace image
{
namespace del
{
class transaction : public elasticsearch::v7::delete_data::transaction
{
public:
    using impl_t = elasticsearch::v7::delete_data::transaction;
    using response = elasticsearch::v7::delete_data::response;

    transaction(const std::string& host);
    ~transaction();

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<response> get_response(Tracer tracer = Tracer()) const;
};
} // namespace del
} // namespace image
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_REQUEST_DELETE_IMAGE_HPP
