#ifndef ELASTICSEARCH_7_10_REQUEST_DELETE_DATA_HPP
#define ELASTICSEARCH_7_10_REQUEST_DELETE_DATA_HPP

#include "adapter/easy/upload_adapter.hpp"
#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "interface_impl/receiver/model_based_sstream_receiver.hpp"
#include "elasticsearch/v7_10/answer_model/delete/model.hpp"
#include "elasticsearch/v7_10/answer_model/delete/deserializer.hpp"
#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/object/Id.h"


// forward declarations
class Connection;

namespace elasticsearch
{
namespace v7
{
namespace delete_data
{
class transaction : public elasticsearch::base::transaction
{
public:
    using base_t = elasticsearch::base::transaction;
    using receiver = ModelBasedSStreamReceiver<response, ResponseFromJSON>;

    static constexpr const char *name() { return "v7/delete_data"; }

    transaction(const std::string& host);
    ~transaction();

    void execute(const std::string& index_name, bool curl_verbose = false);
    void execute_wait_for_refresh(const std::string& index_name, bool curl_verbose = false);
    void execute_force_refresh(const std::string& index_name, bool curl_verbose = false);

    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<response> get_response(Tracer tracer = Tracer{}) const;

private:
    void execute_impl(const std::string& index_name, bool curl_verbose);
    std::shared_ptr<receiver> response_receiver;

    std::shared_ptr<StringUploadDataProvider> source_data;
    std::shared_ptr<adapter::easy::UploadAdapter> uploader;
};
} // namespace delete_data
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_DELETE_DATA_HPP
