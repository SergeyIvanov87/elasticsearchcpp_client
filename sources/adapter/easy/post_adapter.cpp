#include "adapter/easy/post_adapter.hpp"
#include "adapter/easy/POST/adapter_interface.hpp"
#include "adapter/easy/POST/CopyPOSTFields.hpp"
#include "adapter/easy/POST/POSTFields.hpp"
#include "adapter/easy/POST/POSTUpload.hpp"

namespace adapter
{
namespace easy
{
POSTAdapter::POSTAdapter() : connection_handle(nullptr)
{
}

void POSTAdapter::init(CURL *curl_handle)
{
    connection_handle = curl_handle;
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);

    apply_all_adapters([curl_handle] (auto &a)
    {
        a->init(curl_handle);
    });
}

void POSTAdapter::deinit(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_POST, 0L);
    connection_handle = nullptr;
}

void POSTAdapter::reinit(CURL *)
{
    //TODO
}

bool POSTAdapter::has_changed() const
{
    return adaptee_t::has_adapter_changed();
}

POSTAdapter::~POSTAdapter() = default;

CURL* POSTAdapter::get_handle()
{
    return connection_handle;
}
} // namespace easy
} // namespace adapter
