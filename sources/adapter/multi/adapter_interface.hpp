#ifndef ADAPTER_MULTI_MULTI_ADAPTER_INTERFACE_HPP
#define ADAPTER_MULTI_MULTI_ADAPTER_INTERFACE_HPP

#include "sources/adapter/base_adapter_interface.hpp"
#include <curl/curl.h>

namespace adapter
{
struct IMultiAdapter : public IAdapter
{
    virtual ~IMultiAdapter() {};

    virtual void init(CURLM *curl_handle) = 0;
    virtual void deinit(CURLM *curl_handle) = 0;
};
}
#endif // ADAPTER_MULTI_MULTI_ADAPTER_INTERFACE_HPP
