#ifndef INTERFACES_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_UPLOAD_DATA_PROVIDER_HPP

#include "adapter/easy/adapter_interface.hpp"
#include "adapter/base_observer_mixin.hpp"

namespace interface
{
struct IUploadDataProvider
{
    virtual ~IUploadDataProvider() = default;
    virtual size_t fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size) = 0;
    virtual size_t size() const = 0;
    virtual size_t empty() const = 0;
};
}
#endif // INTERFACES_UPLOAD_DATA_PROVIDER_HPP
