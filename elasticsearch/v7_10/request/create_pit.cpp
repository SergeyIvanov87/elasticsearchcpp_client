#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

#include "elasticsearch/v7_10/request/create_pit.hpp"
#include "elasticsearch/v7_10/answer_model/object/Id.hpp"
#include "adapter/easy/curlu_adapter.hpp"
#include "adapter/easy/http/custom_header_adapter.hpp"
#include "adapter/easy/POST/POSTFields.hpp"
#include "sources/connection/connection.hpp"

namespace elasticsearch
{
namespace v7
{
namespace create_pit
{
transaction::transaction(const std::string& host):
    base_t(host, {"Content-Type: application/json","Expect:"}),
    receiver(adapter::IAdapter::create<adapter::easy::StringStreamReceiver>()),
    last_requested_ka("")
{
    connection_ptr->add_adapter(receiver);

    auto post = adapter::IAdapter::create<adapter::easy::POSTAdapter>();
    post->set_adapter(adapter::IAdapter::create<adapter::easy::POSTFieldsAdapter>(std::string("{}")));
    connection_ptr->add_adapter(post);
}

transaction::~transaction() = default;

void transaction::execute(const std::string& index_name, const model::KeepAlive& keep_alive, bool curl_verbose)
{
    receiver->clear();
    url->add_query_param("keep_alive=" + keep_alive.getValue());
    base_t::execute_base(index_name + "/_pit/", curl_verbose);
    last_requested_ka = keep_alive;
}

model::PIT transaction::get_pit() const
{
    nlohmann::json data = nlohmann::json::parse(receiver->get());
    model::PIT pit;
    pit.emplace<model::Id>(data[model::Id::class_name().data()]);
    pit.emplace<model::KeepAlive>(last_requested_ka);
    return pit;
    //return model::Id {data[model::Id::class_name()]};
}
} // namespace create_pit
} // namespace v7
} // namespace elasticsearch
