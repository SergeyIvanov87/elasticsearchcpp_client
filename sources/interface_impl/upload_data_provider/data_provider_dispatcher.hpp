#ifndef INTERFACES_IMPL_DATA_PROVIDER_DISPATCHER_HPP
#define INTERFACES_IMPL_DATA_PROVIDER_DISPATCHER_HPP

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "interfaces/upload_data_provider.hpp"

struct UploadDataProviderDispatcher
{
    static std::shared_ptr<interface::IUploadDataProvider> create(std::string &&string_data,
                                                                  bool rewind_after_end = false);
    static std::shared_ptr<interface::IUploadDataProvider> create(std::vector<unsigned char> &&binary_data,
                                                                  bool rewind_after_end = false);
    static std::shared_ptr<interface::IUploadDataProvider> create(std::istringstream &&stream,
                                                                  bool rewind_after_end = false);
    static std::shared_ptr<interface::IUploadDataProvider> create(std::stringstream &&stream,
                                                                  bool rewind_after_end = false);
    static std::shared_ptr<interface::IUploadDataProvider> create(nlohmann::json &&obj,
                                                                  bool rewind_after_end = false);
};
#endif // INTERFACES_IMPL_DATA_PROVIDER_DISPATCHER_HPP
