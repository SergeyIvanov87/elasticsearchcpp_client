#include <stdexcept>
#include <string>

#include "elasticsearch/v7_10/request/get_data.hpp"

#include "adapter/curlu/url_part_adapter.hpp"
#include "adapter/easy/curlu_adapter.hpp"
#include "adapter/easy/custom_request_adapter.hpp"

#include "sources/connection/connection.hpp"

namespace elasticsearch
{
namespace v7
{
namespace get_json_data
{
transaction::transaction(const std::string& host):
    base_t(host, {"Content-Type: application/json","Expect:"}),
    response_receiver(adapter::IAdapter::create<receiver>())
{
    connection_ptr->add_adapter(response_receiver);
}

transaction::~transaction() = default;

void transaction::execute(const std::string& url_part, bool curl_verbose/* = false*/)
{
    response_receiver->clear();
    base_t::execute_base(url_part, curl_verbose);
}

const transaction::receiver& transaction::get_receiver() const
{
    return *response_receiver;
}

} // namespace get_json_data
} // namespace v7
} // namespace elasticsearch
