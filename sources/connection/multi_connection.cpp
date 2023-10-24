#include "sources/connection/multi_connection.hpp"
#include "sources/connection/connection.hpp"

MultiConnection::MultiConnection() :
    adapter::easy::MultiAdapter()
{
}

MultiConnection::MultiConnection(std::initializer_list<AdapterSharedPtr> list) :
    MultiConnection()
{
    adapters = std::move(list);
    CURLM* multi_handle = get_handle();
    for (auto &a : adapters)
    {
        a->init(multi_handle);
    }
}

MultiConnection::~MultiConnection()
{
    CURLM* multi_handle = get_handle();
    for (auto &a : adapters)
    {
        a->deinit(multi_handle);
    }
}

MultiConnection::Ptr MultiConnection::get_ptr()
{
    return shared_from_this();
}

void MultiConnection::add_connection(ConnectionSharedPtr connection)
{
    if (!connection)
    {
        return;
    }

    SpecificAdapterSharedPtr<adapter::easy::MultiAdapter> multi =
                                std::static_pointer_cast<adapter::easy::MultiAdapter>(this->get_ptr());
    connection->add_adapter(multi);

    try
    {
        connections.insert(connection);
    }
    catch (...)
    {
        connection->remove_adapter(multi);
        throw ;
    }
}

void MultiConnection::remove_connection(ConnectionSharedPtr connection)
{
    auto it = connections.find(connection);
    if (it != connections.end())
    {
        connections.erase(it);
        SpecificAdapterSharedPtr<adapter::easy::MultiAdapter> multi =
                                std::static_pointer_cast<adapter::easy::MultiAdapter>(this->get_ptr());
        connection->remove_adapter(multi);
    }
}

CURLM* MultiConnection::get_handle()
{
    return adapter::easy::MultiAdapter::get_handle();
}

void MultiConnection::wait_all(std::chrono::milliseconds check_interval)
{
    CURLM* multi_handle = get_handle();
    int still_running = 0;
    CURLMcode ret = CURLM_OK;
    do {
        int numfds = 0;
        ret = curl_multi_perform(multi_handle, &still_running);

        if(ret == CURLM_OK) {
            ret = curl_multi_poll(multi_handle, nullptr, 0, check_interval.count(), &numfds);
        }

        if(ret != CURLM_OK) {
            throw std::runtime_error(std::string(__FUNCTION__) +
                                     " - failed with error: " + curl_multi_strerror(ret));
        }
    } while(still_running);
}

int MultiConnection::perform_all(std::chrono::milliseconds wait_ms)
{
    CURLM* multi_handle = get_handle();
    int still_running = 0;
    CURLMcode ret = CURLM_OK;

    int numfds = 0;
    ret = curl_multi_perform(multi_handle, &still_running);

    if(ret == CURLM_OK && still_running != 0) {
        ret = curl_multi_poll(multi_handle, nullptr, 0, wait_ms.count(), &numfds);
    }

    if(ret != CURLM_OK) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 " - failed with error: " + curl_multi_strerror(ret));
    }
    return still_running;
}
