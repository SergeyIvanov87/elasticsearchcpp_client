#include <stdexcept>

#include "adapter/easy/upload_adapter.hpp"
#include "interface_impl/upload_data_provider/binary_array_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/istringstream_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/stringstream_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"

namespace adapter
{
namespace easy
{
UploadAdapter::UploadAdapter(std::shared_ptr<UploaderNotifierInterface> source) :
    base_t(CURLOPT_READFUNCTION, CURLOPT_READDATA),
    data_source_ptr(std::move(source))
{
    if (!data_source_ptr)
    {
        throw std::runtime_error("Cannot create UploadAdapter with an empty 'IUploadDataProvider'");
    }

    if (data_source_ptr->empty())
    {
        throw std::runtime_error("Cannot create UploadAdapter with an empty source");
    }

    data_source_changed = false;
    data_source_ptr->set_notify(this);
}

UploadAdapter::UploadAdapter(std::vector<unsigned char>&& source, bool rewind_after_finish/* = false*/) :
    UploadAdapter(std::make_shared<BinaryArrayUploadDataProvider>(std::move(source), rewind_after_finish))
{
}

UploadAdapter::UploadAdapter(std::string&& source, bool rewind_after_finish/* = false*/) :
    UploadAdapter(std::make_shared<StringUploadDataProvider>(std::move(source), rewind_after_finish))
{
}

UploadAdapter::UploadAdapter(std::istringstream&& source, bool rewind_after_finish/* = false*/) :
    UploadAdapter(std::make_shared<IStringStreamUploadDataProvider>(std::move(source), rewind_after_finish))
{
}

UploadAdapter::UploadAdapter(std::stringstream&& source, bool rewind_after_finish/* = false*/) :
    UploadAdapter(std::make_shared<StringStreamUploadDataProvider>(std::move(source), rewind_after_finish))
{
}

UploadAdapter::UploadAdapter(nlohmann::json&& source, bool rewind_after_finish/* = false*/) :
    UploadAdapter(std::make_shared<JSONUploadDataProvider>(std::move(source), rewind_after_finish))
{
}

void UploadAdapter::init_next(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 1L);
    size_t total_data_size = data_source_ptr->size();
    if (total_data_size > 2 * 1024 * 1024 * 1024ULL)
    {
        curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE, static_cast<curl_off_t>(total_data_size));
    }
    else
    {
        curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE, static_cast<long>(total_data_size));
    }
}

void UploadAdapter::deinit_next(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE, -1L);
    curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE, -1L);
}

void UploadAdapter::reinit_next(CURL *curl_handle)
{
    if (data_source_changed)
    {
        init_next(curl_handle);
        data_source_changed = false;
    }
}

bool UploadAdapter::has_changed_impl() const
{
    return data_source_changed;
}

void UploadAdapter::on_notify_from_impl(interface::IUploadDataProvider&, size_t new_data_size)
{
    (void)new_data_size;
    data_source_changed = true;
}

std::shared_ptr<UploadAdapter::UploaderNotifierInterface>
UploadAdapter::swap(std::shared_ptr<UploaderNotifierInterface> new_source_ptr)
{
    //TODO check in-use
    auto old_source = data_source_ptr;
    std::swap(data_source_ptr, new_source_ptr);

    data_source_changed = true;
    return old_source;
}

size_t UploadAdapter::callback_impl(char *ptr, size_t size, size_t nmemb)
{
    return data_source_ptr->fetch_data(ptr, size * nmemb);
}
} // namespace easy
} // namespase adapter
