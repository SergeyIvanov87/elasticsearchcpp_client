#ifndef ADAPTER_POST_POST_ADAPTER_INTERFACE_HPP
#define ADAPTER_POST_POST_ADAPTER_INTERFACE_HPP

#include "sources/adapter/base_adapter_interface.hpp"
#include <curl/curl.h>

namespace adapter
{
struct IPOSTAdapter : public IAdapter
{
    virtual ~IPOSTAdapter() = default;

    virtual void init(CURL *curl_handle) = 0;
    virtual void deinit(CURL *curl_handle) = 0;

    virtual void reinit(CURL *curl_handle) = 0;
    virtual bool has_changed() const = 0;
};
} // namespace adapter
#endif // ADAPTER_POST_POST_ADAPTER_INTERFACE_HPP
