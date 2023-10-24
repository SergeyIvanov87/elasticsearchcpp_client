#include <stdexcept>
#include <string>

#include "adapter/easy/post_adapter.hpp"
#include "adapter/easy/POST/POSTFields.hpp"

#include "elasticsearch/v7_10/request/search.hpp"
#include "elasticsearch/v7_10/answer_model/search/models.hpp"
#include "elasticsearch/v7_10/answer_model/search/deserializer.hpp"

#include "elasticsearch/v7_10/answer_model/pit/object/Pit.h"
#include "elasticsearch/v7_10/request/create_pit.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
const transaction::receiver& transaction::get_receiver() const
{
    return *response_receiver;
}

transaction::transaction(const std::string& host) :
    base_t(host, {"Content-Type: application/json","Expect:"}),
    response_receiver(adapter::IAdapter::create<receiver>()),
    json_data_provider(),
    uploader()
{
    json_data_provider = std::make_shared<JSONUploadDataProvider>(nlohmann::json::object(), true);
    uploader = adapter::IAdapter::create<adapter::easy::POSTUploadAdapter>(json_data_provider);
    connection_ptr->add_adapter(uploader);
    connection_ptr->add_adapter(response_receiver);

    auto post = adapter::IAdapter::create<adapter::easy::POSTAdapter>();
    post->set_adapter(adapter::IAdapter::create<adapter::easy::POSTFieldsAdapter>(std::string("{}")));
    connection_ptr->add_adapter(post);
}

template<class Tracer>
void transaction::execute_impl(const std::string& index_name, size_t amount, bool curl_verbose, Tracer tracer)
{
    untyped_common_subreq.emplace<::model::Size>(amount);
    {
        CommonSubrequestToJSON serializer;
        auto acc = json_data_provider->access();
        untyped_common_subreq. template format_serialize(serializer, tracer);
        serializer.finalize(acc.object(), tracer);
    }

    response_receiver->clear();
    base_t::execute_base(index_name + "/_search", curl_verbose);
}

template<class Tracer>
void transaction::execute_impl(const std::string& index_name, size_t amount, const ::model::KeepAlive& keep_alive, bool curl_verbose, Tracer tracer)
{
    // Ask PIT
    ::model::PIT pit;
    {
        elasticsearch::v7::create_pit::transaction construct_pit(url->get_url_part());
        construct_pit.execute(index_name, keep_alive, curl_verbose); //// TODO put index in execute_impl()
        pit = construct_pit.get_pit();
    }

    execute_impl(amount, pit, curl_verbose, std::move(tracer));
}

template<class Tracer>
void transaction::execute_impl(size_t amount, const ::model::PIT &pit, bool curl_verbose, Tracer tracer)
{
    untyped_common_subreq.emplace<::model::PIT>(pit);
    execute_impl("", amount, curl_verbose, std::move(tracer));
}


template void transaction::execute_impl(const std::string&, size_t, bool, txml::EmptyTracer);
template void transaction::execute_impl(const std::string&, size_t, bool, txml::StdoutTracer);
template void transaction::execute_impl(const std::string&, size_t, const ::model::KeepAlive&, bool, txml::EmptyTracer);
template void transaction::execute_impl(const std::string&, size_t, const ::model::KeepAlive&, bool, txml::StdoutTracer);
template void transaction::execute_impl(size_t, const ::model::PIT&,bool, txml::EmptyTracer);
template void transaction::execute_impl(size_t, const ::model::PIT&,bool, txml::StdoutTracer);
} // namespace search
} // namespace v7
} // namespace elasticsearch
