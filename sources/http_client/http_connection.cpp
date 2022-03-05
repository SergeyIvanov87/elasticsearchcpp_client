#include "sources/http_client/http_connection.hpp"

namespace http
{
Connection::Connection(std::string_view addr) :
    handle(curl_easy_init(),  curl_easy_cleanup)
{
    curl_easy_setopt(handle.get(), CURLOPT_URL, addr.data());
    curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, static_cast<void*>(this));
    curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, &Connection::write_callback);
}

CURL* Connection::get_handle()
{
    return handle.get();
}

size_t Connection::write_callback(char *ptr, size_t size, size_t nmemb, void *user_data)
{
    Connection* self = static_cast<Connection*>(user_data);
    return self->on_write_data(ptr, size, nmemb);
}
} // http
