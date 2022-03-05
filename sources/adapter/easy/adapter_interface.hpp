#ifndef ADAPTER_EASY_EASY_ADAPTER_INTERFACE_HPP
#define ADAPTER_EASY_EASY_ADAPTER_INTERFACE_HPP

#include "sources/adapter/base_adapter_interface.hpp"
#include <curl/curl.h>

namespace adapter
{
struct IEasyAdapter : public IAdapter
{
    virtual ~IEasyAdapter() = default;

    virtual void init(CURL *curl_handle) = 0;
    virtual void deinit(CURL *curl_handle) = 0;

    virtual void reinit(CURL *curl_handle) = 0;
    virtual bool has_changed() const = 0;
};
}
#endif // ADAPTER_EASY_EASY_ADAPTER_INTERFACE_HPP
