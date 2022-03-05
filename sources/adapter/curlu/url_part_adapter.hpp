#ifndef ADAPTER_CURLU_URL_PART_ADAPTER_HPP
#define ADAPTER_CURLU_URL_PART_ADAPTER_HPP

#include <string>
#include <string_view>

#include "adapter/curlu/adapter_interface.hpp"

namespace adapter
{
namespace curlu
{
class URLPartAdapter: public ICurlUAdapter
{
public:
    using base_t = ICurlUAdapter;

    URLPartAdapter(std::string_view url_part);
    virtual void init(CURLU *curlu_handle) override;
    virtual void deinit(CURLU *curlu_handle) override;

    void set_path(const std::string& path);
    void set_query_param(const std::string& query, unsigned int flags = 0);
    void add_query_param(const std::string& query, unsigned int flags = 0);

    template <class Container>
    void set_query_params(const Container& queries, unsigned int flags = 0)
    {
        set_query_param("");
        bool first_param = true;
        for (auto &&q : queries)
        {
            if (first_param)
            {
                set_query_param(q, flags);
                first_param = false;
                continue;
            }
            add_query_param(q, flags);
        }
    }

    const std::string& get_url_part() const;
    std::string& get_url_part();

    ~URLPartAdapter();
private:
    CURLU *weak_handle;
    std::string url;

    bool query_param_is_set = false; /*provide some optimization for set/add qurey param*/
};
} // namespace curlu
} // namespase adapter
#endif // ADAPTER_CURLU_URL_PART_ADAPTER_HPP
