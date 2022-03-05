#include <stdexcept>

#include "sources/adapter/easy/custom_request_adapter.hpp"

namespace adapter
{
namespace easy
{
CustomRequestAdapter::CustomRequestAdapter(const std::string request) :
    IEasyAdapter(),
    request_name(request)
{
}

CustomRequestAdapter::~CustomRequestAdapter() = default;

void CustomRequestAdapter::init(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, request_name.c_str());
}

void CustomRequestAdapter::deinit(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, nullptr);
}

void CustomRequestAdapter::reinit(CURL *curl_handle)
{
}
bool CustomRequestAdapter::has_changed() const
{
    return false; //until change request_name
}

const std::string &CustomRequestAdapter::get_request_name() const
{
    return request_name;
}
} // namespace easy
} // namespase adapter
