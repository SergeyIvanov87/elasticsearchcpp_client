#ifndef ADAPTER_CURLU_CURLU_ADAPTER_INTERFACE_HPP
#define ADAPTER_CURLU_CURLU_ADAPTER_INTERFACE_HPP

#include "sources/adapter/base_adapter_interface.hpp"
#include <curl/curl.h>

namespace adapter
{
struct ICurlUAdapter : public IAdapter
{
    virtual ~ICurlUAdapter() {};

    virtual void init(CURLU *curlu_handle) = 0;
    virtual void deinit(CURLU *curlu_handle) = 0;
};
} // namespace adapter
#endif // ADAPTER_CURLU_CURLU_ADAPTER_INTERFACE_HPP
