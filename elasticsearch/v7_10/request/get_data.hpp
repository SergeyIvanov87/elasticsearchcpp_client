#ifndef ELASTICSEARCH_7_10_REQUEST_GET_DATA_HPP
#define ELASTICSEARCH_7_10_REQUEST_GET_DATA_HPP

#include <txml/txml_fwd.h>

#include "adapter/easy/upload_adapter.hpp"

#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "adapter/easy/sstream_receiver.hpp"
#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/models.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/deserializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace get_json_data
{
class transaction : public elasticsearch::base::transaction
{
public:
    using base_t = elasticsearch::base::transaction;
    using receiver =  adapter::easy::StringStreamReceiver;

    static constexpr const char *name() { return "v7/get_json_data"; }

    transaction(const std::string& host);
    ~transaction();

    void execute(const std::string& url_part, bool curl_verbose = false);
    const receiver& get_receiver() const;

    template<class ResponseModel, template<typename> class ...ResponseDeserializer, class Tracer = txml::EmptyTracer>
    std::optional<response<ResponseModel>> get_response(Tracer tracer = Tracer{}) const
    {
        std::string received_string = get_receiver().get();
        nlohmann::json json_data = nlohmann::json::parse(received_string);
        ResponseDeSerializer<ResponseModel, ResponseDeserializer...> deserializer(json_data);
        std::optional<response<ResponseModel>> resp_ptr = response<ResponseModel>::template format_deserialize(deserializer, tracer);
        if (!resp_ptr)
        {
            throw std::runtime_error(std::string("Cannot deserialize response: ") + ResponseModel::class_name().data());
        }

        return resp_ptr;
    }

private:
    std::shared_ptr<receiver> response_receiver;
};
} // namespace get_json_data
} // namespace v7
} // namespace elasticsearch

#endif // ELASTICSEARCH_7_10_REQUEST_GET_DATA_HPP
