#ifndef ELASTICSEARCH_7_10_REQUEST_PUT_DATA_HPP
#define ELASTICSEARCH_7_10_REQUEST_PUT_DATA_HPP

#include <txml/txml_fwd.h>

#include "adapter/easy/upload_adapter.hpp"
#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"
#include "adapter/easy/sstream_receiver.hpp"

#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/models.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/serializer.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/deserializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping {
class transaction;
}
namespace put_json_data
{
class transaction : public elasticsearch::base::transaction
{
public:
    friend class elasticsearch::v7::index_mapping::transaction;

    using base_t = elasticsearch::base::transaction;
    using receiver =  adapter::easy::StringStreamReceiver;

    static constexpr const char *name() { return "v7/put_json_data"; }

    template<class MappingModel, template<class> class ...MappingModelSerializer>
    struct Tag {
        using model_type = MappingModel;

        Tag(const model_type &model) : instance(model) {}
        const model_type& instance;
    };
/*
    template<class MappingModel, template<typename> class ...MappingModelSerializer, class Tracer = txml::EmptyTracer>
    transaction(const std::string& host, Tag<MappingModel, MappingModelSerializer...> &&tag,
                bool rewind_data_after_finish = false, Tracer tracer = Tracer()) :
        transaction(host, std::true_type{})
    {
        RequestSerializer<MappingModel, MappingModelSerializer...> serializer;
        tag.instance.template format_serialize(serializer, tracer);
        json_data_provider = std::make_shared<JSONUploadDataProvider>(serializer.finalize(tracer), rewind_data_after_finish);
        uploader = adapter::IAdapter::create<adapter::easy::UploadAdapter>(json_data_provider);

        //uploader = adapter::IAdapter::create<adapter::easy::UploadAdapter>(serializer.dump(), rewind_data_after_finish);
        connection_ptr->add_adapter(uploader);
    }

*/
    transaction(const std::string& host, bool rewind_data_after_finish = false) :
        transaction(host, std::true_type{})
    {
        json_data_provider = std::make_shared<JSONUploadDataProvider>(nlohmann::json::object(), rewind_data_after_finish);
        uploader = adapter::IAdapter::create<adapter::easy::UploadAdapter>(json_data_provider);
        connection_ptr->add_adapter(uploader);
    }

    ~transaction() = default;


    template<class MappingModel, template<typename> class ...MappingModelSerializer, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, const Tag<MappingModel, MappingModelSerializer...> &t,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_model_params_impl(t, tracer);
        execute_impl(index_name, curl_verbose);
    }

    template<class MappingModel, template<typename> class ...MappingModelSerializer, class Tracer = txml::EmptyTracer>
    void execute_wait_for_refresh(const std::string& index_name, const Tag<MappingModel, MappingModelSerializer...> &t,
                                  bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_model_params_impl(t, tracer);
        execute_wait_for_refresh_impl(index_name, curl_verbose);
    }

    template<class MappingModel, template<typename> class ...MappingModelSerializer, class Tracer = txml::EmptyTracer>
    void execute_force_refresh(const std::string& index_name, const Tag<MappingModel, MappingModelSerializer...> &t,
                               bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_model_params_impl(t, tracer);
        execute_force_refresh_impl(index_name, curl_verbose);
    }

    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer = Tracer{}) const;

private:
    void execute_impl(const std::string& index_name, bool curl_verbose = false);
    void execute_wait_for_refresh_impl(const std::string& index_name, bool curl_verbose = false);
    void execute_force_refresh_impl(const std::string& index_name, bool curl_verbose = false);

    template<class RequestSerializer>
    transaction(const std::string& host, const RequestSerializer &out, bool rewind_data_after_finish = false) :
        transaction(host, std::true_type{})
    {
        json_data_provider = std::make_shared<JSONUploadDataProvider>(out.finalize(), rewind_data_after_finish);
        uploader = adapter::IAdapter::create<adapter::easy::UploadAdapter>(json_data_provider);
        connection_ptr->add_adapter(uploader);
    }

    template<class MappingModel, template<typename> class ...MappingModelSerializer,
             class Tracer = txml::EmptyTracer>
    void serialize_model_params_impl(const Tag<MappingModel, MappingModelSerializer...> &t,
                                     Tracer tracer = Tracer())
    {
        RequestSerializer<MappingModel, MappingModelSerializer...> serializer;
        t.instance.template format_serialize(serializer, tracer);
        {
            auto acc = json_data_provider->access();
            serializer.finalize(acc.object(), tracer);
        }
    }

    transaction(const std::string& host, std::true_type);

    std::shared_ptr<receiver> response_receiver;
    std::shared_ptr<JSONUploadDataProvider> json_data_provider;
    std::shared_ptr<adapter::easy::UploadAdapter> uploader;
};
} // namespace put_json_data
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_PUT_DATA_HPP
