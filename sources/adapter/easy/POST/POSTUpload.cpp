#include <stdexcept>

#include "adapter/easy/POST/POSTUpload.hpp"
#include "interface_impl/upload_data_provider/binary_array_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/istringstream_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/stringstream_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"

namespace adapter
{
namespace easy
{
    /*
POSTUploadAdapter::POSTUploadAdapter(std::shared_ptr<interface::IUploadDataProvider> source) :
    base_t(CURLOPT_READFUNCTION, CURLOPT_READDATA),
    data_source_ptr(std::move(source))
{
    if (!data_source_ptr)
    {
        throw std::runtime_error("Cannot create POSTUploadAdapter with an empty 'IUploadDataProvider'");
    }

    if (data_source_ptr->empty())
    {
        throw std::runtime_error("Cannot create POSTUploadAdapter with an empty source");
    }

    data_source_changed = false;
    //data_source_ptr->set_notify(this);
}
*/
POSTUploadAdapter::POSTUploadAdapter(std::shared_ptr<UploaderNotifierInterface> source) :
    base_t(CURLOPT_READFUNCTION, CURLOPT_READDATA),
    data_source_ptr(std::move(source))
{
    if (!data_source_ptr)
    {
        assert(false && "data_source_ptr is nullptr");
        throw std::runtime_error("Cannot create POSTUploadAdapter with an empty 'IUploadDataProvider'");
    }

    if (data_source_ptr->empty())
    {
        assert(false && "data_source_ptr is empty");
        throw std::runtime_error("Cannot create POSTUploadAdapter with an empty source");
    }

    data_source_changed = false;
    data_source_ptr->set_notify(this);
}


POSTUploadAdapter::POSTUploadAdapter(std::vector<unsigned char>&& source, bool rewind_after_finish/* = false*/) :
    POSTUploadAdapter(std::make_shared<BinaryArrayUploadDataProvider>(std::move(source), rewind_after_finish))
{
    //std::dynamic_pointer_cast<BinaryArrayUploadDataProvider>(data_source_ptr)->set_notify(this);
}

POSTUploadAdapter::POSTUploadAdapter(std::string&& source, bool rewind_after_finish/* = false*/) :
    POSTUploadAdapter(std::make_shared<StringUploadDataProvider>(std::move(source), rewind_after_finish))
{
    //std::dynamic_pointer_cast<StringUploadDataProvider>(data_source_ptr)->set_notify(this);
}

POSTUploadAdapter::POSTUploadAdapter(std::istringstream&& source, bool rewind_after_finish/* = false*/) :
    POSTUploadAdapter(std::make_shared<IStringStreamUploadDataProvider>(std::move(source), rewind_after_finish))
{
    //std::dynamic_pointer_cast<IStringStreamUploadDataProvider>(data_source_ptr)->set_notify(this);
}

POSTUploadAdapter::POSTUploadAdapter(std::stringstream&& source, bool rewind_after_finish/* = false*/) :
    POSTUploadAdapter(std::make_shared<StringStreamUploadDataProvider>(std::move(source), rewind_after_finish))
{
    //std::dynamic_pointer_cast<StringStreamUploadDataProvider>(data_source_ptr)->set_notify(this);
}

POSTUploadAdapter::POSTUploadAdapter(nlohmann::json&& source, bool rewind_after_finish/* = false*/) :
    POSTUploadAdapter(std::make_shared<JSONUploadDataProvider>(std::move(source), rewind_after_finish))
{
    //std::dynamic_pointer_cast<StringStreamUploadDataProvider>(data_source_ptr)->set_notify(this);
}

void POSTUploadAdapter::init_next(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS , nullptr);
    size_t total_data_size = data_source_ptr->size();
    if (total_data_size > 2 * 1024 * 1024 * 1024ULL)
    {
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(total_data_size));
    }
    else
    {
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, static_cast<long>(total_data_size));
    }
}

void POSTUploadAdapter::deinit_next(CURL *curl_handle)
{
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE_LARGE, -1L);
}

void POSTUploadAdapter::reinit_next(CURL *curl_handle)
{
    if (data_source_changed)
    {
        init_next(curl_handle);
        data_source_changed = false;
    }
}

bool POSTUploadAdapter::has_changed_impl() const
{
    return data_source_changed;
}


void POSTUploadAdapter::on_notify_from_impl(interface::IUploadDataProvider&, size_t new_data_size)
{
    (void)new_data_size;
    data_source_changed = true;
    /*
    if (new_data_size > 2 * 1024 * 1024 * 1024ULL)
    {
        curl_easy_setopt(nullptr, CURLOPT_POSTFIELDSIZE, static_cast<curl_off_t>(new_data_size));
    }
    else
    {
        curl_easy_setopt(nullptr, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<long>(new_data_size));
    }*/
}
/*
std::shared_ptr<interface::IUploadDataProvider>
POSTUploadAdapter::reset(std::shared_ptr<interface::IUploadDataProvider> new_source_ptr)
{
    //TODO check in-use
    auto old_source = data_source_ptr;
    std::swap(data_source_ptr, new_source_ptr);

    data_source_changed = true;
    return old_source;
}
*/
std::shared_ptr<POSTUploadAdapter::UploaderNotifierInterface>
POSTUploadAdapter::reset(std::shared_ptr<UploaderNotifierInterface> new_source_ptr)
{
    //TODO check in-use
    auto old_source = data_source_ptr;
    std::swap(data_source_ptr, new_source_ptr);

    data_source_changed = true;
    return old_source;
}
size_t POSTUploadAdapter::callback_impl(char *ptr, size_t size, size_t nmemb)
{
    return data_source_ptr->fetch_data(ptr, size * nmemb);
}
} // namespace easy
} // namespase adapter
