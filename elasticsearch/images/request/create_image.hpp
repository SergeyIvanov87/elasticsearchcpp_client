#ifndef ELASTICSEARCH_IMAGE_REQUEST_CREATE_BOOK_HPP
#define ELASTICSEARCH_IMAGE_REQUEST_CREATE_BOOK_HPP

#include "elasticsearch/v7_10/request/put_data.hpp"
#include "elasticsearch/images/data_model/model.hpp"

namespace elasticsearch
{
namespace image
{
namespace create
{
class transaction
{
public:
    using impl_t = elasticsearch::v7::put_json_data::transaction;
    using receiver = impl_t::receiver;
    using response = elasticsearch::v7::put_json_data::response;

    static constexpr const char *name() { return "image/create"; }

    transaction(const std::string& host);

    ~transaction();


    template<class Tracer = txml::EmptyTracer>
    void execute_force_refresh(const std::string& index_name, const elasticsearch::image::model::data &instance,
                               bool curl_verbose = false, Tracer tracer = Tracer());

    template<class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, const elasticsearch::image::model::data &instance,
                 bool curl_verbose = false, Tracer tracer = Tracer());

    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<response> get_response(Tracer tracer = Tracer()) const;
private:
    std::unique_ptr<impl_t> generic_transaction_ptr;
};
} // namespace create
} // namespace image
} // namespace elasticsearch
#endif // ELASTICSEARCH_IMAGE_REQUEST_CREATE_BOOK_HPP
