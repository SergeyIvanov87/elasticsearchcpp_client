#include <algorithm>

#include "interface_impl/upload_data_provider/istringstream_upload_data_provider.hpp"

IStringStreamUploadDataProvider::IStringStreamUploadDataProvider(std::istringstream &&data,
                                                                 bool rewind_after_finish) :
    IStreamUploadDataProvider(nullptr, rewind_after_finish),
    stringstream(std::move(data))
{
    IStreamUploadDataProvider::reset(&stringstream, rewind_after_finish);
}

IStringStreamUploadDataProvider::~IStringStreamUploadDataProvider() = default;

void IStringStreamUploadDataProvider::swap(std::istringstream& new_data, bool rewind_after_finish)
{
    std::swap(stringstream, new_data);
    IStreamUploadDataProvider::reset(&stringstream, rewind_after_finish);
}
