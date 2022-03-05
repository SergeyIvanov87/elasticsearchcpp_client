#include "interface_impl/upload_data_provider/data_provider_dispatcher.hpp"
#include "interface_impl/upload_data_provider/istringstream_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/stringstream_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/binary_array_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"


std::shared_ptr<interface::IUploadDataProvider>
UploadDataProviderDispatcher::create(std::string &&string_data,
                                     bool rewind_after_end)
{
    return std::make_shared<StringUploadDataProvider>(std::forward<std::string>(string_data),
                                                      rewind_after_end);
}

std::shared_ptr<interface::IUploadDataProvider>
UploadDataProviderDispatcher::create(std::vector<unsigned char> &&binary_data,
                                     bool rewind_after_end)
{
    return std::make_shared<BinaryArrayUploadDataProvider>(std::vector<unsigned char>(binary_data),
                                                           rewind_after_end);
}


std::shared_ptr<interface::IUploadDataProvider>
UploadDataProviderDispatcher::create(std::istringstream &&stream,
                                     bool rewind_after_end)
{
    return std::make_shared<IStringStreamUploadDataProvider>(std::forward<std::istringstream>(stream),
                                                             rewind_after_end);
}


std::shared_ptr<interface::IUploadDataProvider>
UploadDataProviderDispatcher::create(std::stringstream &&stream,
                                     bool rewind_after_end)
{
    return std::make_shared<StringStreamUploadDataProvider>(std::forward<std::stringstream>(stream),
                                                            rewind_after_end);
}

std::shared_ptr<interface::IUploadDataProvider>
UploadDataProviderDispatcher::create(nlohmann::json &&obj,
                                     bool rewind_after_end)
{
    return std::make_shared<JSONUploadDataProvider>(std::move(obj), rewind_after_end);
}
