#ifndef CONNECTION_MULTI_CONNECTION_HPP
#define CONNECTION_MULTI_CONNECTION_HPP

#include <chrono>
#include <initializer_list>
#include <memory>
#include <set>
#include <string_view>
#include <type_traits>

#include <curl/curl.h>

#include "adapter/multi/adapter_interface.hpp"
#include "adapter/easy/multi_adapter.hpp"

class Connection;
class MultiConnection : public adapter::easy::MultiAdapter,
                        public std::enable_shared_from_this<MultiConnection>
{
public:
    using Ptr = std::shared_ptr<MultiConnection>;

    using AdapterSharedPtr = adapter::IAdapter::SharedPtr<adapter::IMultiAdapter>;

    template<class SpecificAdapter>
    using SpecificAdapterSharedPtr = adapter::IAdapter::SharedPtr<SpecificAdapter>;

    using ConnectionSharedPtr = std::shared_ptr<Connection>;

    template<class ...Args>
    static Ptr create(Args &&...args)
    {
        return Ptr { new MultiConnection(std::forward<Args>(args)...) };
    }

    ~MultiConnection();

    Ptr get_ptr();
    void add_connection(ConnectionSharedPtr connection);
    void remove_connection(ConnectionSharedPtr connection);
    CURLM* get_handle();
    void wait_all(std::chrono::milliseconds check_interval = std::chrono::milliseconds(0));
    int perform_all(std::chrono::milliseconds wait_ms);
protected:
    MultiConnection();

    template<class ...Adapters>
    MultiConnection(Adapters&& ...args) :
        MultiConnection(std::initializer_list<AdapterSharedPtr> {std::dynamic_pointer_cast<adapter::IMultiAdapter>(args)...})
    {
        static_assert(
                std::conjunction_v<std::is_same<SpecificAdapterSharedPtr<typename std::decay<Adapters>::type::element_type>,
                                                typename std::decay<Adapters>::type> ...>,
                "`Adapters` must be correct `adapter::IAdapter::SharedPtr` type");

        static_assert(
                std::conjunction_v<std::is_base_of<adapter::IMultiAdapter,
                                                   typename std::decay<Adapters>::type::element_type> ...>,
                "'Adapters' must implement `adapter::IMultiAdapter`");

        static_assert(
                std::conjunction_v<std::negation<
                                        std::is_same<typename std::decay<Adapters>::type::element_type,
                                                     adapter::easy::MultiAdapter>>...>,
                "`Adapters` must not be `adapter::easy::MultiAdapter` type");

    }

    MultiConnection(std::initializer_list<AdapterSharedPtr> list);

private:
    std::set<AdapterSharedPtr> adapters;
    std::set<ConnectionSharedPtr> connections;
};
#endif // CONNECTION_MULTI_CONNECTION_HPP
