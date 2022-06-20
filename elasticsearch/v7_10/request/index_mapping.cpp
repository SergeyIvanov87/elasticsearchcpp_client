#include <stdexcept>
#include <string>

#include "elasticsearch/v7_10/answer_model/index_mapping/deserializer.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping
{

void transaction::execute(const std::string& index_name, bool curl_verbose/* = false*/)
{
    generic_transaction_ptr->execute_impl(index_name, curl_verbose);
}

adapter::curlu::URLPartAdapter &transaction::get_url_part_adapter()
{
    return generic_transaction_ptr->get_url_part_adapter();
}

const transaction::receiver& transaction::get_receiver() const
{
    return generic_transaction_ptr->get_receiver();
}

template<class Tracer>
std::optional<transaction::response> transaction::get_response(Tracer tracer) const
{
    std::string received_string = get_receiver().get();
    nlohmann::json json_data = nlohmann::json::parse(received_string);

    elasticsearch::v7::index_mapping::ResponseFromJSON in(json_data);
    std::optional<response> resp_ptr = response::format_deserialize(in, tracer);
    if (!resp_ptr)
    {
        throw std::runtime_error(std::string("Cannot deserialize response: ") + response::class_name().data());
    }

    return resp_ptr;
}

template std::optional<transaction::response> transaction::get_response(txml::StdoutTracer) const;
template std::optional<transaction::response> transaction::get_response(txml::EmptyTracer) const;
} // namespace index_mapping
} // namespace v7
} // namespace elasticsearch
