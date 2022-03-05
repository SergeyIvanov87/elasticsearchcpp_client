#ifndef ADAPTER_EASY_HTTP_CUSTOM_HEADER_ADAPTER_HPP
#define ADAPTER_EASY_HTTP_CUSTOM_HEADER_ADAPTER_HPP

#include "adapter/easy/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
namespace http
{
class CustomHeader final: public IEasyAdapter
{
public:
    using base_t = IEasyAdapter;

    CustomHeader(std::initializer_list<const char*> headers_list = {});
    ~CustomHeader();
    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;

    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;

    void add_header(const char *line);
private:
    struct curl_slist *headers;
};
} // namespace http
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_HTTP_CUSTOM_HEADER_ADAPTER_HPP
