#include <stdexcept>
#include <string>

#include "elasticsearch/v7_10/request/delete_pit.hpp"
#include "elasticsearch/v7_10/answer_model/object/Id.hpp"

#include "sources/connection/connection.hpp"
#include "sources/adapter/easy/custom_request_adapter.hpp"

namespace elasticsearch
{
namespace v7
{
namespace delete_pit
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

void transaction::execute(const model::Id& pit, bool curl_verbose/* = false*/)
{
    response_receiver->clear();

    std::string pit_json("{\"");
    pit_json = pit_json + model::Id::class_name().data() + "\" : \"" + pit.value() + "\"}";

    if (!uploader)
    {
        source_data = std::make_shared<StringUploadDataProvider>(std::move(pit_json));
        uploader = adapter::IAdapter::create<adapter::easy::UploadAdapter>(source_data);
        connection_ptr->add_adapter(uploader);
    }
    else
    {
        source_data->swap(pit_json);
    }

    base_t::execute_base("/_pit", curl_verbose);
}

const transaction::receiver& transaction::get_receiver() const
{
    return *response_receiver;
}

template<class Tracer>
response transaction::get_response(Tracer tracer) const
{
    std::string received_string = get_receiver().get();
    nlohmann::json json_data = nlohmann::json::parse(received_string);
    ResponseFromJSON in(json_data);
    return get_receiver().response(in, tracer);
}

template response transaction::get_response(txml::StdoutTracer) const;
template response transaction::get_response(txml::EmptyTracer) const;
} // namespace delete_pit
} // namespace v7
} // namespace elasticsearch
