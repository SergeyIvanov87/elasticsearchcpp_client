#ifndef ELASTICSEARCH_7_10_REQUEST_DELETE_PIT_HPP
#define ELASTICSEARCH_7_10_REQUEST_DELETE_PIT_HPP

#include "adapter/easy/upload_adapter.hpp"
#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "interface_impl/receiver/model_based_sstream_receiver.hpp"

#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/pit/delete_pit_model.hpp"
#include "elasticsearch/v7_10/answer_model/pit/deserializer.hpp"
#include "elasticsearch/v7_10/answer_model/object/Id.h"


// forward declarations
class Connection;

namespace elasticsearch
{
namespace v7
{
namespace delete_pit
{
class transaction : public elasticsearch::base::transaction
{
public:
    using base_t = elasticsearch::base::transaction;
    using receiver = ModelBasedSStreamReceiver<response, ResponseFromJSON>;

    static constexpr const char *name() { return "v7/delete_pit"; }

    transaction(const std::string& host);
    ~transaction();

    void execute(const model::Id& pit, bool curl_verbose = false);
    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer = Tracer()) const;
private:
    std::shared_ptr<receiver> response_receiver;

    std::shared_ptr<StringUploadDataProvider> source_data;
    std::shared_ptr<adapter::easy::UploadAdapter> uploader;
};
} // namespace delete_pit
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_DELETE_PIT_HPP
