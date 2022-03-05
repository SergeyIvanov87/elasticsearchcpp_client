#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_BINARY_ARRAY_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_BINARY_ARRAY_UPLOAD_DATA_PROVIDER_HPP

#include <vector>

#include "interface_impl/upload_data_provider/detail/plain_container_provider_impl.hpp"
#include "adapter/easy/upload_adapter.hpp"
#include "adapter/easy/POST/POSTUpload.hpp"


struct BinaryArrayUploadDataProvider final :
     public interface::INotifierDataProviderDispatcher<interface::IUploadDataProvider,
                                                       adapter::easy::UploadAdapter,
                                                       adapter::easy::POSTUploadAdapter>
{
    BinaryArrayUploadDataProvider(std::vector<unsigned char> &&data, bool rewind_after_finish = false);
    ~BinaryArrayUploadDataProvider();

    size_t fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size) override;
    size_t size() const override;
    size_t empty() const override;

    void swap(std::vector<unsigned char>& new_data, bool rewind_after_finish = false);
private:
    PlainContainerUploadDataProvider<std::vector<unsigned char>,
                                     adapter::easy::UploadAdapter,
                                     adapter::easy::POSTUploadAdapter> impl;
};

#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_BINARY_ARRAY_UPLOAD_DATA_PROVIDER_HPP
