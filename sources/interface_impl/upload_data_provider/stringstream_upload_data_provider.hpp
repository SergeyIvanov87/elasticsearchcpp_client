#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_STRING_STREAM_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_STRING_STREAM_UPLOAD_DATA_PROVIDER_HPP

#include <sstream>

#include "interface_impl/upload_data_provider/istream_upload_data_provider.hpp"

struct StringStreamUploadDataProvider final : public IStreamUploadDataProvider
{
    StringStreamUploadDataProvider(std::stringstream&& stream, bool rewind_after_finish = false);
    ~StringStreamUploadDataProvider();

    void swap(std::stringstream& new_data, bool rewind_after_finish);

private:
    mutable std::stringstream stringstream;
};

#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_ISTREAM_UPLOAD_DATA_PROVIDER_HPP
