#include "adapter/easy/POST/POSTFields.hpp"

#include "adapter/curlu/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
POSTFieldsAdapter::POSTFieldsAdapter(std::string &&fields) :
    post_fields(std::move(fields))
{}

POSTFieldsAdapter::~POSTFieldsAdapter() = default;

void POSTFieldsAdapter::init(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post_fields.data());
}

void POSTFieldsAdapter::deinit(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, nullptr);
}

void POSTFieldsAdapter::reinit(CURL *curl_handle)
{
    init(curl_handle);
}
bool POSTFieldsAdapter::has_changed() const
{
    return false;
}
} // namespace easy
} // namespase adapter
