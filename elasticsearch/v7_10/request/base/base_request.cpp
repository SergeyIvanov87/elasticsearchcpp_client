#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "adapter/easy/http/custom_header_adapter.hpp"

namespace elasticsearch
{
namespace base
{
transaction::transaction(const std::string& host, std::initializer_list<const char*> custom_headers_list) :
    connection_ptr(),
    url(adapter::IAdapter::create<adapter::curlu::URLPartAdapter>(host))
{
    auto url_adapter = adapter::IAdapter::create<adapter::easy::CurlUAdapter>(url);
    if (!std::empty(custom_headers_list))
    {
        auto header = adapter::IAdapter::create<adapter::easy::http::CustomHeader>(custom_headers_list);
                    //std::initializer_list<const char*>{"Content-Type: application/json","Expect:"});

        connection_ptr = Connection::create<Connection>("", url_adapter, header);
    }
    else
    {
        connection_ptr = Connection::create<Connection>("", url_adapter);
    }
}

transaction::~transaction() = default;

void transaction::set_pretty(bool enable)
{
    if (enable)
    {
        get_url_part_adapter().add_query_param("pretty=true");
    }
    else
    {
        get_url_part_adapter().add_query_param("pretty=false");
    }
}

adapter::curlu::URLPartAdapter &transaction::get_url_part_adapter()
{
    return *url;
}

void transaction::execute_base(const std::string& url_part, bool curl_verbose)
{
    get_url_part_adapter().set_path(url_part);
    CURLcode res = connection_ptr->perform(curl_verbose);
    if(res != CURLE_OK)
    {
        throw std::runtime_error(std::string(__FUNCTION__) + " - failed: " +
                                 curl_easy_strerror(res) + ", code: " + std::to_string(res));
    }
}
} // namespace base
} // namespace elasticsearch
