#ifndef ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_DELETE_HPP
#define ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_DELETE_HPP

#include "interface_impl/receiver/model_based_sstream_receiver.hpp"

#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/index_mapping_delete/models.hpp"
#include "elasticsearch/v7_10/answer_model/index_mapping_delete/deserializer.hpp"

class Connection;
namespace elasticsearch
{
namespace v7
{
namespace index_mapping_delete
{
class transaction: public elasticsearch::base::transaction
{
public:
    using base_t = elasticsearch::base::transaction;
    using receiver = ModelBasedSStreamReceiver<response, ResponseFromJSON>;

    static constexpr const char *name() { return "v7/index_mapping_delete"; }

    transaction(const std::string& host);
    ~transaction() = default;

    void execute(const std::string& index_name, bool curl_verbose = false);
    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer) const;
private:
    std::shared_ptr<receiver> response_receiver;
};
} // namespace index_mapping_delete
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_DELETE_HPP
