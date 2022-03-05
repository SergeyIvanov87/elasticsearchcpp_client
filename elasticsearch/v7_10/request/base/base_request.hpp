#ifndef ELASTICSEARCH_BASE_BASE_REQUEST_HPP
#define ELASTICSEARCH_BASE_BASE_REQUEST_HPP

#include "adapter/curlu/url_part_adapter.hpp"
#include "adapter/easy/curlu_adapter.hpp"
#include "sources/connection/connection.hpp"

namespace elasticsearch
{
namespace base
{
class transaction
{
public:
    transaction(const std::string& host, std::initializer_list<const char*> custom_headers_list = {});
    adapter::curlu::URLPartAdapter &get_url_part_adapter();
    void set_pretty(bool enable);
protected:
    ~transaction();
    void execute_base(const std::string& url_part, bool curl_verbose);
    std::unique_ptr<Connection> connection_ptr;
    std::shared_ptr<adapter::curlu::URLPartAdapter> url;
};
} // namespace base
} // namespace elasticsearch

#endif // ELASTICSEARCH_BASE_BASE_REQUEST_HPP
