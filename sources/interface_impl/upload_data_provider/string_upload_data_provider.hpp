#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_STRING_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_STRING_UPLOAD_DATA_PROVIDER_HPP
#include <string>

#include "interface_impl/upload_data_provider/detail/plain_container_provider_impl.hpp"
#include "adapter/easy/upload_adapter.hpp"
#include "adapter/easy/POST/POSTUpload.hpp"

struct StringUploadDataProvider final :
     public interface::INotifierDataProviderDispatcher<interface::IUploadDataProvider,
                                                       adapter::easy::UploadAdapter,
                                                       adapter::easy::POSTUploadAdapter>
{
    StringUploadDataProvider(std::string &&data, bool rewind_after_finish = false);
    StringUploadDataProvider(const std::string &str, bool rewind_after_finish = false);
    ~StringUploadDataProvider();

    size_t fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size) override;
    size_t size() const override;
    size_t empty() const override;

    void swap(std::string& new_data, bool rewind_after_finish = false);
private:
    PlainContainerUploadDataProvider<std::string,
                                     adapter::easy::UploadAdapter,
                                     adapter::easy::POSTUploadAdapter> impl;

};

#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_STRING_UPLOAD_DATA_PROVIDER_HPP
