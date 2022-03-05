#ifndef ADAPTER_EASY_CUSTOM_REQUEST_ADAPTER_HPP
#define ADAPTER_EASY_CUSTOM_REQUEST_ADAPTER_HPP
#include <sstream>

#include "adapter/easy/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
class CustomRequestAdapter: public IEasyAdapter
{
public:
    using base_t = IEasyAdapter;

    CustomRequestAdapter(const std::string request);
    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;
    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;

    const std::string &get_request_name() const;

    ~CustomRequestAdapter();
private:
    std::string request_name;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_CUSTOM_REQUEST_ADAPTER_HPP
