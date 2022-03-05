#include <stdexcept>

#include "sources/adapter/easy/curlu_adapter.hpp"

namespace adapter
{
namespace easy
{
CurlUAdapter::CurlUAdapter() :
    IEasyAdapter(),
    curlu_handle(curl_url(),  curl_url_cleanup)
{
}

CurlUAdapter::~CurlUAdapter()
{
}

void CurlUAdapter::init(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_CURLU, curlu_handle.get());
}

void CurlUAdapter::deinit(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_CURLU, nullptr);
}

void CurlUAdapter::reinit(CURL *curl_handle)
{
}

bool CurlUAdapter::has_changed() const
{
    return false;
}

CURLU* CurlUAdapter::get_handle()
{
    return curlu_handle.get();
}
} // namespace easy
} // namespase adapter
