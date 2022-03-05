#include "sources/connection/connection.hpp"

Connection::Connection() :
    handle(curl_easy_init(), curl_easy_cleanup)
{
}

Connection::Connection(std::string_view addr, std::initializer_list<AdapterSharedPtr> list) :
    Connection()
{
    curl_easy_setopt(handle.get(), CURLOPT_URL, addr.data());

    for (auto &a : list)
    {
        add_adapter_impl(a);
    }
}

Connection::~Connection()
{
    for (auto &a : adapters)
    {
        a->deinit(handle.get());
    }
    adapters.clear();
}

CURL* Connection::get_handle()
{
    return handle.get();
}

CURLcode Connection::perform(bool curl_verbose /* = false */)
{
    // test on adapter data changed
    for (auto &adapter : adapters)
    {
        if (adapter->has_changed())
        {
            adapter->reinit(get_handle());
        }
    }
    curl_easy_setopt(get_handle(), CURLOPT_VERBOSE, curl_verbose ? 1L : 0L);
    return curl_easy_perform(get_handle());
}

void Connection::add_adapter_impl(AdapterSharedPtr adapt)
{
    adapt->init(handle.get());
    adapters.insert(adapt);
}

void Connection::remove_adapter_impl(AdapterSharedPtr adapt)
{
    auto it = adapters.find(adapt);
    if (it != adapters.end())
    {
        (*it)->deinit(handle.get());
        adapters.erase(it);
    }
}
