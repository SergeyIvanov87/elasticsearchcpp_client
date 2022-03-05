#ifndef ADAPTER_ADAPTER_INTERFACE_HPP
#define ADAPTER_ADAPTER_INTERFACE_HPP

#include <memory>
#include <type_traits>

namespace adapter
{
struct IAdapter
{
    using Ptr = std::shared_ptr<IAdapter>;

    template<class SpecificAdapter>
    using SharedPtr = std::shared_ptr<SpecificAdapter>;

    virtual ~IAdapter() = default;

    template<class SpecificAdapter, class ...Args>
    static SharedPtr<SpecificAdapter> create(Args &&...args)
    {
        static_assert(std::is_base_of_v<IAdapter, SpecificAdapter>, "Adapterr must inherit IAdapter");
        return SharedPtr<SpecificAdapter> { new SpecificAdapter(std::forward<Args>(args)...) };
    }
};
}
#endif // ADAPTER_ADAPTER_INTERFACE_HPP
