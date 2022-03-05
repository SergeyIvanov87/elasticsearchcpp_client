#ifndef CONNECTION_CONNECTION_HPP
#define CONNECTION_CONNECTION_HPP

#include <initializer_list>
#include <memory>
#include <set>
#include <string_view>
#include <type_traits>

#include <curl/curl.h>

#include "adapter/easy/adapter_interface.hpp"
#include "adapter/easy/adapter_callback_interface.hpp"

class Connection
{
public:
    template<class SpecificConnectionType>
    using Ptr = std::unique_ptr<SpecificConnectionType>;

    using AdapterSharedPtr = adapter::IAdapter::SharedPtr<adapter::IEasyAdapter>;

    template<class SpecificAdapter>
    using SpecificAdapterSharedPtr = adapter::IAdapter::SharedPtr<SpecificAdapter>;

    using UniqueCURLHandle = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)>;

    template<class SpecificConnectionType, class ...Args>
    static Ptr<SpecificConnectionType> create(Args &&...args)
    {
        return Ptr<SpecificConnectionType> { new SpecificConnectionType(std::forward<Args>(args)...) };
    }

    ~Connection();

    CURL* get_handle();
    CURLcode perform(bool curl_verbose = false);

    template<class SpecificAdapter>
    void add_adapter(SpecificAdapterSharedPtr<SpecificAdapter> adapt) {
        static_assert(std::is_base_of_v<adapter::IEasyAdapter, SpecificAdapter>,
                      "'Adapters' must implement `adapter::IEasyAdapter`");
        add_adapter_impl(std::dynamic_pointer_cast<adapter::IEasyAdapter>(adapt));
    }

    template<class SpecificAdapter>
    void remove_adapter(SpecificAdapterSharedPtr<SpecificAdapter> adapt) {
        static_assert(std::is_base_of_v<adapter::IEasyAdapter, SpecificAdapter>,
                      "'Adapters' must implement `adapter::IEasyAdapter`");
        remove_adapter_impl(std::dynamic_pointer_cast<adapter::IEasyAdapter>(adapt));
    }
protected:
    Connection();

    template<class ...Adapters>
    Connection(std::string_view addr, Adapters&& ...args) :
        Connection(addr,
                   std::initializer_list<AdapterSharedPtr> {
                                std::dynamic_pointer_cast<adapter::IEasyAdapter>(args)...})
    {
        static_assert(std::conjunction_v<std::is_same<SpecificAdapterSharedPtr<typename std::decay<Adapters>::type::element_type>,
                                                      typename std::decay<Adapters>::type> ...>,
                      "`Adapters` must be correct `adapter::IEasyAdapter::SharedPtr` type");
        static_assert(std::conjunction_v<std::is_base_of<adapter::IEasyAdapter,
                                                         typename std::decay<Adapters>::type::element_type> ...>,
                      "'Adapters' must implement `adapter::IEasyAdapter`");
    }

    Connection(std::string_view addr, std::initializer_list<AdapterSharedPtr> list = {});

    void add_adapter_impl(AdapterSharedPtr adapt);
    void remove_adapter_impl(AdapterSharedPtr adapt);
private:
    UniqueCURLHandle handle;
    std::set<AdapterSharedPtr> adapters;
};
#endif // CONNECTION_CONNECTION_HPP
