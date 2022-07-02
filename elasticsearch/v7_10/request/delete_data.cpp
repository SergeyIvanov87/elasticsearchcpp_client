#include <stdexcept>
#include <string>

#include "elasticsearch/v7_10/request/delete_data.hpp"
#include "elasticsearch/v7_10/answer_model/object/Id.hpp"

#include "sources/connection/connection.hpp"
#include "sources/adapter/easy/custom_request_adapter.hpp"

namespace elasticsearch
{
namespace v7
{
namespace delete_data
{
transaction::transaction(const std::string& host):
    base_t(host, {"Content-Type: application/json","Expect:"}),
    response_receiver(adapter::IAdapter::create<ModelBasedSStreamReceiver<response, ResponseFromJSON>>()),
    source_data(),
    uploader()
{
    connection_ptr->add_adapter(adapter::IAdapter::create<adapter::easy::CustomRequestAdapter>("DELETE"));
    connection_ptr->add_adapter(response_receiver);
}

transaction::~transaction() = default;

void transaction::execute(const std::string& index_name, bool curl_verbose)
{
    execute_impl(index_name, curl_verbose);
}

void transaction::execute_wait_for_refresh(const std::string& index_name, bool curl_verbose)
{
    url->add_query_param("refresh=wait_for");
    execute_impl(index_name, curl_verbose);
}

void transaction::execute_force_refresh(const std::string& index_name, bool curl_verbose)
{
    url->add_query_param("refresh=true");
    execute_impl(index_name, curl_verbose);
}

void transaction::execute_impl(const std::string& index_name, bool curl_verbose)
{
    response_receiver->clear();
    base_t::execute_base(index_name, curl_verbose);
}

const transaction::receiver& transaction::get_receiver() const
{
    return *response_receiver;
}

template<class Tracer>
response transaction::get_response(Tracer tracer) const
{
    //-S- todo hide in model based receiver
    std::string received_string = get_receiver().get();
    nlohmann::json json_data = nlohmann::json::parse(received_string);
    ResponseFromJSON in(json_data);
    return get_receiver().response(in, tracer);
}

template response transaction::get_response(txml::StdoutTracer) const;
template response transaction::get_response(txml::EmptyTracer) const;
} // namespace delete_data
} // namespace v7
} // namespace elasticsearch
