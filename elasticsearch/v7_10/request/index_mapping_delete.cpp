#include <stdexcept>
#include <string>

#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"

#include "adapter/curlu/url_part_adapter.hpp"
#include "adapter/easy/curlu_adapter.hpp"
#include "adapter/easy/custom_request_adapter.hpp"

#include "sources/connection/connection.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping_delete
{
transaction::transaction(const std::string& host):
    base_t(host),
    response_receiver(adapter::IAdapter::create<receiver>())
{
    connection_ptr->add_adapter(adapter::IAdapter::create<adapter::easy::CustomRequestAdapter>("DELETE"));
    connection_ptr->add_adapter(response_receiver);
}

void transaction::execute(const std::string& index_name, bool curl_verbose/* = false*/)
{
    response_receiver->clear();
    base_t::execute_base(index_name, curl_verbose);
}

const transaction::receiver& transaction::get_receiver() const
{
    return *response_receiver;
}

template<class Tracer>
std::shared_ptr<response> transaction::get_response(Tracer tracer) const
{
    // TODO move it out to ModelBasedSStrem receiver!!!
    std::string received_string = get_receiver().get();
    nlohmann::json json_data = nlohmann::json::parse(received_string);
    ResponseFromJSON in(json_data);
    return get_receiver().response(in, tracer);
}

template std::shared_ptr<response> transaction::get_response(txml::StdoutTracer) const;
template std::shared_ptr<response> transaction::get_response(txml::EmptyTracer) const;
} // namespace index_mapping_delete
} // namespace v7
} // namespace elasticsearch
