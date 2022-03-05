#include "sources/http_client/http_get.hpp"

namespace http
{
Get::Get(std::string_view addr) :
    Connection(addr)
{
}

size_t Get::on_write_data(char *ptr, size_t size, size_t nmemb)
{
    resp.write(ptr, size * nmemb);
    return size * nmemb;
}

std::string Get::get() const
{
    return resp.str();
}
}
