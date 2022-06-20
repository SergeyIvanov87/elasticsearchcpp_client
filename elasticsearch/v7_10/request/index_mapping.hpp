#ifndef ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_HPP
#define ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_HPP

#include "elasticsearch/v7_10/request/put_data.hpp"

#include "elasticsearch/v7_10/answer_model/index_mapping/index_mapping_models.hpp"
#include "elasticsearch/v7_10/answer_model/index_mapping/serializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping
{
class transaction
{
public:
    using impl_t = elasticsearch::v7::put_json_data::transaction;
    using receiver = impl_t::receiver;
    using response = elasticsearch::v7::index_mapping::response;

    static constexpr const char *name() { return "v7/index_mapping"; }

    template<class MappingModel, template<class> class ...MappingModelSerializer>
    struct Tag {
        using model_type = MappingModel;
    };

    template<class MappingModel, template<typename> class ...MappingModelSerializer, class Tracer = txml::EmptyTracer>
    transaction(const std::string& host, Tag<MappingModel, MappingModelSerializer...>,
                bool rewind_data_after_finish = false, Tracer tracer = Tracer())
    {
        // Inject application domain model into IM request
        RequestSerializer<MappingModel, MappingModelSerializer...> serializer;
        request<MappingModel>::template schema_serialize(serializer, tracer);

        // Redirect IM request to base transaction put data
        generic_transaction_ptr.reset(new impl_t(host, serializer, rewind_data_after_finish));
    }

    ~transaction() = default;

    void execute(const std::string& index_name, bool curl_verbose = false);
    adapter::curlu::URLPartAdapter &get_url_part_adapter();
    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer = Tracer()) const;
private:
    std::unique_ptr<impl_t> generic_transaction_ptr;
};
} // namespace index_mapping
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_INDEX_MAPPING_HPP
