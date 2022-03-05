#include <stdlib.h> // TODO abort()


#include "adapter/easy/POST/CopyPOSTFields.hpp"

#include "adapter/curlu/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
CopyPOSTFieldsAdapter::CopyPOSTFieldsAdapter(std::string_view fields) :
    post_fields(fields)
{}

CopyPOSTFieldsAdapter::~CopyPOSTFieldsAdapter() = default;

void CopyPOSTFieldsAdapter::init(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, post_fields.data());
}

void CopyPOSTFieldsAdapter::deinit(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, nullptr);
}

void CopyPOSTFieldsAdapter::reinit(CURL *curl_handle)
{
    init(curl_handle);
}
bool CopyPOSTFieldsAdapter::has_changed() const
{
    return false;
}
} // namespace easy
} // namespase adapter
