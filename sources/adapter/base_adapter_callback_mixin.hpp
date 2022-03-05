#ifndef ADAPTER_BASE_ADAPTER_CALLBACK_MIXIN_HPP
#define ADAPTER_BASE_ADAPTER_CALLBACK_MIXIN_HPP

#include <curl/curl.h>

#include "adapter/base_adapter_interface.hpp"

namespace adapter
{
template<class Impl, class ...CallbackArgs>
struct ICallbackAdapter
{
    const void* get_user_data() const
    {
        return static_cast<const Impl*>(this);
    }

    auto get_callback() const// -> decltype(&ICallbackAdapter::callback)
    {
        return &ICallbackAdapter::callback;
    }

    CURLoption get_cb_opt_id() const
    {
        return curl_callback_opt_id;
    }

    CURLoption get_cb_data_opt_id() const
    {
        return curl_callback_data_opt_id;
    }

protected:
    ICallbackAdapter(CURLoption callback_curl_option_id, CURLoption callback_data_curl_option_id) :
        curl_callback_opt_id(callback_curl_option_id),
        curl_callback_data_opt_id(callback_data_curl_option_id)
    {}
    ~ICallbackAdapter() {};

    static size_t callback(CallbackArgs... args, void* user_data)
    {
        Impl* self = static_cast<Impl*>(user_data);
        return self->callback_impl(args...);
    }

private:
    CURLoption curl_callback_opt_id;
    CURLoption curl_callback_data_opt_id;
};
}
#endif // ADAPTER_BASE_ADAPTER_CALLBACK_MIXIN_HPP
