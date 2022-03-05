#include <algorithm>

#include "interface_impl/upload_data_provider/stringstream_upload_data_provider.hpp"

StringStreamUploadDataProvider::StringStreamUploadDataProvider(std::stringstream &&data,
                                                               bool rewind_after_finish) :
    IStreamUploadDataProvider(nullptr, rewind_after_finish),
    stringstream(std::move(data))
{
    IStreamUploadDataProvider::reset(&stringstream, rewind_after_finish);
}

StringStreamUploadDataProvider::~StringStreamUploadDataProvider() = default;

void StringStreamUploadDataProvider::swap(std::stringstream& new_data, bool rewind_after_finish)
{
    std::swap(stringstream, new_data);
    IStreamUploadDataProvider::reset(&stringstream, rewind_after_finish);
}
