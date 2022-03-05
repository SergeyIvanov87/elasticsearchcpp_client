#ifndef ADAPTER_POST_COPY_POST_FIELDS_ADAPTER_HPP
#define ADAPTER_POST_COPY_POST_FIELDS_ADAPTER_HPP
#include <string_view>
#include <string>
#include "adapter/easy/POST/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
class CopyPOSTFieldsAdapter: public IPOSTAdapter
{
public:
    using base_t = IPOSTAdapter;

    CopyPOSTFieldsAdapter(std::string_view fields);
    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;
    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;
    ~CopyPOSTFieldsAdapter();
private:
    std::string post_fields;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_POST_COPY_POST_FIELDS_ADAPTER_HPP
