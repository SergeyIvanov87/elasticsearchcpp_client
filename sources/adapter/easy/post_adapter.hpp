#ifndef ADAPTER_EASY_POST_ADAPTER_HPP
#define ADAPTER_EASY_POST_ADAPTER_HPP

#include "adapter/easy/adapter_interface.hpp"
#include "adapter/base_exclusive_adaptee_mixin.hpp"

namespace adapter
{
class IPOSTAdapter;
namespace easy
{
class POSTUploadAdapter;
class POSTFieldsAdapter;
class CopyPOSTFieldsAdapter;
class POSTAdapter: public IEasyAdapter,
                   public IExclusiveAdaptee<POSTAdapter, IPOSTAdapter,
                                            POSTUploadAdapter, POSTFieldsAdapter, CopyPOSTFieldsAdapter>
{
public:
    using base_t = IEasyAdapter;
    using adaptee_t = IExclusiveAdaptee<POSTAdapter, IPOSTAdapter,
                                        POSTUploadAdapter, POSTFieldsAdapter, CopyPOSTFieldsAdapter>;


    POSTAdapter();

    template<class ...Adapters>
    POSTAdapter(Adapters &&...adapters) : POSTAdapter() {
        adaptee_t::template set_adapters(std::forward<Adapters>(adapters)...);
    }

    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;
    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;

    CURL* get_handle();
    ~POSTAdapter();
private:
    CURL* connection_handle;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_POST_ADAPTER_HPP
