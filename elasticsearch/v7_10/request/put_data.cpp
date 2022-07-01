#include <stdexcept>
#include <string>

#include "elasticsearch/v7_10/request/put_data.hpp"

#include "adapter/curlu/url_part_adapter.hpp"
#include "adapter/easy/curlu_adapter.hpp"
#include "adapter/easy/custom_request_adapter.hpp"

#include "sources/connection/connection.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/models.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/deserializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace put_json_data
{
void transaction::execute_wait_for_refresh_impl(const std::string& index_name, bool curl_verbose)
{
    url->add_query_param("refresh=wait_for");
    execute_impl(index_name, curl_verbose);
}

void transaction::execute_force_refresh_impl(const std::string& index_name, bool curl_verbose)
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
    std::string received_string = get_receiver().get();
    nlohmann::json json_data = nlohmann::json::parse(received_string);
    elasticsearch::v7::put_json_data::ResponseFromJSON in(json_data);

    std::optional<response> resp = response::format_deserialize(in, tracer);
    if (!resp)
    {
        std::stringstream ss;
        ss << "Cannot deserialize response: " << response::class_name()
           << ". Parse trace: ";
        tracer.dump(ss);
        throw std::runtime_error(ss.str());
    }

    return resp.value();
}

transaction::transaction(const std::string& host, std::true_type) :
    base_t(host, {"Content-Type: application/json","Expect:"}),
    response_receiver(adapter::IAdapter::create<receiver>()),
    uploader()
{
    connection_ptr->add_adapter(response_receiver);
}

template response transaction::get_response(txml::StdoutTracer) const;
template response transaction::get_response(txml::EmptyTracer) const;
} // namespace put_json_data
} // namespace v7
} // namespace elasticsearch
