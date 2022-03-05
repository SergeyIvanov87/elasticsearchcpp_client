#ifndef ADAPTER_POST_POST_FIELDS_ADAPTER_HPP
#define ADAPTER_POST_POST_FIELDS_ADAPTER_HPP
#include <string>

#include "adapter/easy/POST/adapter_interface.hpp"

namespace adapter
{
namespace easy
{
class POSTFieldsAdapter: public IPOSTAdapter
{
public:
    using base_t = IPOSTAdapter;

    POSTFieldsAdapter(std::string&& fields);
    virtual void init(CURL *curl_handle) override;
    virtual void deinit(CURL *curl_handle) override;
    virtual void reinit(CURL *curl_handle) override;
    virtual bool has_changed() const override;

    ~POSTFieldsAdapter();
private:
    std::string post_fields;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_POST_POST_FIELDS_ADAPTER_HPP
