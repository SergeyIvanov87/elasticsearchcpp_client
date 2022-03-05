#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTREAM_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTREAM_UPLOAD_DATA_PROVIDER_HPP

#include <istream>

#include "interfaces/notifier_data_provider_dispatcher.hpp"
#include "interfaces/upload_data_provider.hpp"

namespace adapter {
    namespace easy {
        class UploadAdapter;
        class POSTUploadAdapter;
    }
}

struct IStreamUploadDataProvider :
        public interface::INotifierDataProviderDispatcher<interface::IUploadDataProvider,
                                                          adapter::easy::UploadAdapter,
                                                          adapter::easy::POSTUploadAdapter>
{
    IStreamUploadDataProvider(std::istream* stream, bool rewind_after_finish = false);
    ~IStreamUploadDataProvider();

    virtual size_t fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size) override;
    virtual size_t size() const override;
    virtual size_t empty() const override;

    void reset(std::istream* new_data, bool rewind_after_finish);
private:
    std::istream* stream_ptr;
    bool rewind_stream;
};

#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTREAM_UPLOAD_DATA_PROVIDER_HPP
