#ifndef HTTP_CLIENT_HTTP_GET_HPP
#define HTTP_CLIENT_HTTP_GET_HPP

#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include "sources/http_client/http_connection.hpp"

namespace http
{
class Get : public Connection
{
public:
    friend class Connection;
    std::string get() const;

protected:
    Get(std::string_view addr);
    size_t on_write_data(char *ptr, size_t size, size_t nmemb) override;

    std::stringstream resp;
};
}

#endif // HTTP_CLIENT_HTTP_GET_HPP
