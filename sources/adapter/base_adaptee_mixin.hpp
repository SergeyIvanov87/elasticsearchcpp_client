#ifndef ADAPTER_BASE_ADAPTEE_MIXIN_HPP
#define ADAPTER_BASE_ADAPTEE_MIXIN_HPP

#include <algorithm>
#include <array>
#include <tuple>
#include <type_traits>

#include <curl/curl.h>

#include "adapter/base_adapter_interface.hpp"

namespace adapter
{
template<class Impl, class BaseAdapter, class ...Adapters>
struct IAdaptee
{
    using AdapterType = BaseAdapter;

    using AdapterSharedPtr = adapter::IAdapter::SharedPtr<AdapterType>;

    template<class SpecificAdapter>
    using SpecificAdapterSharedPtr = adapter::IAdapter::SharedPtr<SpecificAdapter>;

    using AdaptersTuple = std::tuple<SpecificAdapterSharedPtr<Adapters>...>;

    template<class SpecificAdapter>
    void set_adapter(SpecificAdapterSharedPtr<SpecificAdapter> adapt) {
        static_assert(std::is_base_of_v<AdapterType, SpecificAdapter>,
                      "'SpecificAdapter' must implement `AdapterType`");
        // TODO static check on available adapters

        Impl* self = static_cast<Impl*>(this);
        auto& current_adapter = std::get<SpecificAdapterSharedPtr<SpecificAdapter>>(adapters);
        if (current_adapter)
        {
            if (adapt)
            {
                self->swap_adapter_impl(current_adapter, adapt);
            }
            else
            {
                self->remove_adapter_impl(current_adapter);
                current_adapter.reset();
            }
        }
        else
        {
            if (adapt)
            {
                self->add_adapter_impl(adapt);
                std::swap(current_adapter, adapt);
            }
        }
    }

    template<class ...SpecificAdapters>
    void set_adapters(SpecificAdapterSharedPtr<SpecificAdapters>...adapters)
    {
        std::array<bool, sizeof...(SpecificAdapters)> expander {(set_adapter(adapters), true)...};
        (void)expander;
    }

    template <class Functor>
    size_t apply_all_adapters(Functor f)
    {
        return std::apply([&f] (const SpecificAdapterSharedPtr<Adapters> &...adapter) -> size_t
        {
            bool dispatchingResult[]
            {
                (!adapter ? false : (f(adapter), true))...
            };
            // success if one of element has changed
            return std::count_if(dispatchingResult, dispatchingResult + sizeof...(Adapters), [] (bool val) {
                return val;
            });
        }, adapters);
    }
protected:
    template<class SpecificAdapter>
    void swap_adapter_impl(SpecificAdapterSharedPtr<SpecificAdapter> &current_adapt,
                           SpecificAdapterSharedPtr<SpecificAdapter> &cand_adapt)
    {
        Impl* self = static_cast<Impl*>(this);
        auto* handle = self->get_handle();

        current_adapt->deinit(handle);
        cand_adapt->init(handle);

        std::swap(current_adapt, cand_adapt);
    }

    template<class SpecificAdapter>
    void add_adapter_impl(SpecificAdapterSharedPtr<SpecificAdapter> &new_adapt)
    {
        Impl* self = static_cast<Impl*>(this);
        auto* handle = self->get_handle();

        new_adapt->init(handle);
    }

    template<class SpecificAdapter>
    void remove_adapter_impl(SpecificAdapterSharedPtr<SpecificAdapter> &current_adapter)
    {
        Impl* self = static_cast<Impl*>(this);
        auto* handle = self->get_handle();

        current_adapter->deinit(handle);
    }

private:
    AdaptersTuple adapters;
};
}
#endif // ADAPTER_BASE_ADAPTEE_MIXIN_HPP
