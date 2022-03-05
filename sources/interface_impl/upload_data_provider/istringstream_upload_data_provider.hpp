#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTRING_STREAM_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTRING_STREAM_UPLOAD_DATA_PROVIDER_HPP

#include <sstream>

#include "interface_impl/upload_data_provider/istream_upload_data_provider.hpp"

struct IStringStreamUploadDataProvider final : public IStreamUploadDataProvider
{
    IStringStreamUploadDataProvider(std::istringstream&& stream, bool rewind_after_finish = false);
    ~IStringStreamUploadDataProvider();

    void swap(std::istringstream& new_data, bool rewind_after_finish);

private:
    mutable std::istringstream stringstream;
};

#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTREAM_UPLOAD_DATA_PROVIDER_HPP
