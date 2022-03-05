#ifndef ADAPTER_EASY_EASY_ADAPTER_CALLBACK_INTERFACE_HPP
#define ADAPTER_EASY_EASY_ADAPTER_CALLBACK_INTERFACE_HPP

#include "adapter/base_adapter_callback_mixin.hpp"
#include "adapter/easy/adapter_interface.hpp"

namespace adapter
{
template<class Impl, bool ImplOverrideBehavior, class ...CallbackArgs>
struct IEasyCallbackAdapter : public IEasyAdapter, public ICallbackAdapter<Impl, CallbackArgs...>
{
    friend class IEasyAdapter;

    virtual ~IEasyCallbackAdapter() = default;

    virtual void init(CURL *curl_handle) override
    {
        Impl* self = static_cast<Impl*>(this);
        curl_easy_setopt(curl_handle, this->get_cb_data_opt_id(), this->get_user_data());
        curl_easy_setopt(curl_handle, this->get_cb_opt_id(), this->get_callback());

        if constexpr(ImplOverrideBehavior == true) {
            static_cast<Impl*>(this)->init_next(curl_handle);
        }
    }

    virtual void deinit(CURL *curl_handle) override
    {
        curl_easy_setopt(curl_handle, this->get_cb_data_opt_id(), nullptr);
        curl_easy_setopt(curl_handle, this->get_cb_opt_id(), nullptr);

        if constexpr(ImplOverrideBehavior == true) {
            static_cast<Impl*>(this)->deinit_next(curl_handle);
        }
    }

    virtual void reinit(CURL *curl_handle) override
    {
        if constexpr(ImplOverrideBehavior == true) {
            static_cast<Impl*>(this)->reinit_next(curl_handle);
        }
    }

    virtual bool has_changed() const override
    {
        if constexpr(ImplOverrideBehavior == true) {
            return static_cast<const Impl*>(this)->has_changed_impl();
        }
        return false;
    }
protected:
    IEasyCallbackAdapter(CURLoption callback_curl_option_id, CURLoption callback_data_curl_option_id) :
        IEasyAdapter(), ICallbackAdapter<Impl, CallbackArgs...>(callback_curl_option_id, callback_data_curl_option_id)
    {}
};
}
#endif // ADAPTER_EASY_EASY_ADAPTER_CALLBACK_INTERFACE_HPP
