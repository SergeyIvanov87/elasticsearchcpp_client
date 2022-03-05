#ifndef ADAPTER_EASY_MULTI_ADAPTER_HPP
#define ADAPTER_EASY_MULTI_ADAPTER_HPP
#include <sstream>

#include "adapter/easy/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
class MultiAdapter: public IEasyAdapter
{
public:
    using base_t = IEasyAdapter;
    using UniqueCURLMultiHandle = std::unique_ptr<CURLM, decltype(&curl_multi_cleanup)>;

    MultiAdapter();
    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;

    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;

    CURLM* get_handle();

protected:
    ~MultiAdapter();
private:
    UniqueCURLMultiHandle multi_handle;
    std::size_t easy_handles_count;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_MULTI_ADAPTER_HPP
