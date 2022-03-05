#ifndef ADAPTER_BASE_EXCLUSIVE_ADAPTEE_MIXIN_HPP
#define ADAPTER_BASE_EXCLUSIVE_ADAPTEE_MIXIN_HPP

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include <curl/curl.h>

#include "adapter/base_adapter_interface.hpp"

namespace adapter
{
namespace details
{
template<int I>
constexpr int tuple_index_impl_failed()
{
    static_assert(I != 0, "Cannot find TupleElement");
    return I;
}

template<int I, class Type, class FirstElement, class ...Elements>
struct tuple_index_impl {
    static constexpr int value = std::is_same_v<Type, FirstElement> ?
                                    I :
                                tuple_index_impl<I + 1, Type, Elements...>::value;
};

template<int I, class Type, class LastElement>
struct tuple_index_impl<I, Type, LastElement> {
    static constexpr int value = std::is_same_v<Type, LastElement> ?
                                    I : std::numeric_limits<int>::max();
                                //tuple_index_impl_failed<0>();
};


template<class Type, class ...TupleElements>
struct tuple_index {
    static constexpr int value = tuple_index_impl<0, typename std::decay<Type>::type, TupleElements...>::value;
};
}

template<class Impl, class BaseAdapter, class ...ExclusiveAdapters>
struct IExclusiveAdaptee
{
    using AdapterType = BaseAdapter;

    using AdapterSharedPtr = adapter::IAdapter::SharedPtr<AdapterType>;

    template<class SpecificAdapter>
    using SpecificAdapterSharedPtr = adapter::IAdapter::SharedPtr<SpecificAdapter>;

    using ExclusiveAdaptersTuple = std::tuple<SpecificAdapterSharedPtr<ExclusiveAdapters>...>;

    template<class SpecificAdapter>
    void set_adapter(SpecificAdapterSharedPtr<SpecificAdapter> adapt) {
        static_assert(std::is_base_of_v<AdapterType, SpecificAdapter>,
                      "'SpecificAdapter' must implement `AdapterType`");
        // TODO static check on available adapters
        constexpr int specific_adapter_index = details::tuple_index<SpecificAdapter, ExclusiveAdapters...>::value;
        if (set_adapter_index != std::numeric_limits<int>::max() &&
            set_adapter_index != specific_adapter_index)
        {
            throw std::runtime_error("Cannot set SpecificAdapter because one ExclusiveAdapters has been set already");
        }

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
        return std::apply([&f] (const SpecificAdapterSharedPtr<ExclusiveAdapters> &...adapter) -> size_t
        {
            bool dispatchingResult[]
            {
                (!adapter ? false : (f(adapter), true))...
            };
            // success if one of element has changed
            return std::count_if(dispatchingResult, dispatchingResult + sizeof...(ExclusiveAdapters), [] (bool val) {
                return val;
            });
        }, adapters);
    }

    bool has_adapter_changed() const
    {
        return std::apply([] (const SpecificAdapterSharedPtr<ExclusiveAdapters> &...adapter) -> bool
        {
            bool dispatchingResult[]
            {
                (!adapter ? false : adapter->has_changed())...
            };
            // success if one of element has changed
            return std::any_of(dispatchingResult, dispatchingResult + sizeof...(ExclusiveAdapters), [] (bool val) {
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
    ExclusiveAdaptersTuple adapters;

    int set_adapter_index = std::numeric_limits<int>::max();
};
}
#endif // ADAPTER_BASE_EXCLUSIVE_ADAPTEE_MIXIN_HPP
