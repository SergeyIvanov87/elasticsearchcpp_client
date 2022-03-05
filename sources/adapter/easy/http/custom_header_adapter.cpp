#include <stdexcept>

#include "sources/adapter/easy/http/custom_header_adapter.hpp"

namespace adapter
{
namespace easy
{
namespace http
{
CustomHeader::CustomHeader(std::initializer_list<const char*> headers_list) :
    IEasyAdapter(),
    headers()
{
    for (auto h : headers_list)
    {
        headers = curl_slist_append(headers, h);
    }

}

CustomHeader::~CustomHeader()
{
    curl_slist_free_all(headers);
}

void CustomHeader::init(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
}

void CustomHeader::deinit(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, nullptr);
}

void CustomHeader::add_header(const char *line)
{
    headers = curl_slist_append(headers, line);
}

void CustomHeader::reinit(CURL *curl_handle)
{
}
bool CustomHeader::has_changed() const
{
    return false; // append headers linst doesn't require reinit
}

} // namespace http
} // namespace easy
} // namespase adapter
