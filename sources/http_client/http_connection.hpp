#ifndef HTTP_CLIENT_HTTP_CONNECTION_HPP
#define HTTP_CLIENT_HTTP_CONNECTION_HPP

#include <memory>
#include <string_view>


#include <curl/curl.h>

namespace http
{
class Connection
{
public:
    template<class SpecificConnectionType>
    using Ptr = std::shared_ptr<SpecificConnectionType>;

    using UniqueCURLHandle = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)>;

    template<class SpecificConnectionType, class ...Args>
    static Ptr<SpecificConnectionType> create(Args &&...args)
    {
        return Ptr<SpecificConnectionType> { new SpecificConnectionType(std::forward<Args>(args)...) };
    }

    ~Connection() = default;

    CURL* get_handle();

protected:
    Connection(std::string_view addr);

private:
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    virtual size_t on_write_data(char *ptr, size_t size, size_t nmemb) = 0;

    UniqueCURLHandle handle;
};
} // http
#endif // HTTP_CLIENT_HTTP_CONNECTION_HPP
