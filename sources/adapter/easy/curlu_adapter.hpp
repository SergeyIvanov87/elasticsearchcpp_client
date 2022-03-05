#ifndef ADAPTER_EASY_CURLU_ADAPTER_HPP
#define ADAPTER_EASY_CURLU_ADAPTER_HPP
#include <sstream>

#include "adapter/easy/adapter_interface.hpp"
#include "adapter/base_adaptee_mixin.hpp"

namespace adapter
{
class ICurlUAdapter;
namespace curlu
{
    class URLPartAdapter;
}
namespace easy
{
class CurlUAdapter: public IEasyAdapter,
                    public IAdaptee<CurlUAdapter, ICurlUAdapter,
                                                  curlu::URLPartAdapter>
{
public:
    using base_t = IEasyAdapter;
    using adaptee_t = IAdaptee<CurlUAdapter, ICurlUAdapter,
                                             curlu::URLPartAdapter>;

    using UniqueCURLUHandle = std::unique_ptr<CURLU, decltype(&curl_url_cleanup)>;

    CurlUAdapter();

    template<class ...Adapters>
    CurlUAdapter(Adapters &&...adapters) : CurlUAdapter() {
        adaptee_t::template set_adapters(std::forward<Adapters>(adapters)...);
    }

    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;
    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;

    CURLU* get_handle();
    ~CurlUAdapter();
private:
    UniqueCURLUHandle curlu_handle;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_CURLU_ADAPTER_HPP
