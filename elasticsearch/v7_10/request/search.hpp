#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_NEW_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_NEW_HPP

#include <txml/txml_fwd.h>

#include "adapter/easy/POST/POSTUpload.hpp"
#include "adapter/easy/sstream_receiver.hpp"

#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"
#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"

#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/search/serializer.hpp"
#include "elasticsearch/v7_10/answer_model/search/deserializer.hpp"

#include "elasticsearch/v7_10/request/tags/search/tags.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
class transaction : public elasticsearch::base::transaction
{
public:
    using base_t = elasticsearch::base::transaction;
    using receiver =  adapter::easy::StringStreamReceiver;

    static constexpr const char *name() { return "v7/search"; }

    transaction(const std::string& host);
/*
    template<class Model, class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    transaction(const std::string& host, const tag::query<Model, SpecificQueryParams...> &t, Tracer tracer = Tracer()) :
        transaction(host)
    {
        {
            auto acc = json_data_provider->access();
            t.serialize(acc.object(), tracer);
        }
    }
*/
    ~transaction() = default;

    /************* execute request to find records matching with specific query params ************/
    template <class Model, class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        execute_impl(index_name, amount, curl_verbose, tracer);
    }

    template<class Model, class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        execute_impl(index_name, amount, keep_alive, curl_verbose, tracer);
    }

    template<class Model, class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(size_t amount,
                 const ::model::PIT &pit,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        execute_impl(amount, pit, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class Model, class ...SpecificQueryParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        execute_impl(index_name, amount, ::model::KeepAlive {keep_alive}, curl_verbose, tracer);
    }

    /*********************** execute request to find all available records ************************/
    template <class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        execute(index_name, amount, tag::query_all {}, curl_verbose, tracer);
    }

    template<class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        execute(index_name, amount, keep_alive, tag::query_all {}, curl_verbose, tracer);
    }

    template<class Tracer = txml::EmptyTracer>
    void execute(size_t amount,
                 const ::model::PIT &pit,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        execute(amount, pit, tag::query_all {}, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        execute(index_name, amount, keep_alive, tag::query_all {}, curl_verbose, tracer);
    }

    /* execute request to find records matching with specific query params and sorted by specific fields */
    template <class Model,
              class ...SpecificQueryParams, class ...SortParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        serialize_sort_params_impl(s, tracer);
        execute_impl(index_name, amount, curl_verbose, tracer);
    }

    template<class Model,
             class ...SpecificQueryParams, class ...SortParams,
             class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        serialize_sort_params_impl(s, tracer);
        execute_impl(index_name, amount, keep_alive, curl_verbose, tracer);
    }


    template<class Model,
             class ...SpecificQueryParams, class ...SortParams,
             class Tracer = txml::EmptyTracer>
    void execute(size_t amount, const ::model::PIT &pit,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        serialize_sort_params_impl(s, tracer);
        execute_impl(amount, pit, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class Model,
              class ...SpecificQueryParams, class ...SortParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::query<Model, SpecificQueryParams...> &q,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_query_params_impl(q, tracer);
        serialize_sort_params_impl(s, tracer);
        execute_impl(index_name, amount, ::model::KeepAlive {keep_alive}, curl_verbose, tracer);
    }


    /********* execute_impl request to find all available records and sorted by specific fields ********/
    template <class Model, class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_sort_params_impl(s, tracer);
        execute(index_name, amount, tag::query_all {}, curl_verbose, tracer);
    }

    template<class Model, class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_sort_params_impl(s, tracer);
        execute(amount, keep_alive, tag::query_all {}, curl_verbose, tracer);
    }


    template<class Model, class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(size_t amount, const ::model::PIT &pit,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_sort_params_impl(s, tracer);
        execute(amount, pit, tag::query_all {}, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class Model, class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::sort<Model, SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        serialize_sort_params_impl(s, tracer);
        execute(index_name, amount, keep_alive, tag::query_all {}, curl_verbose, tracer);
    }

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
    std::shared_ptr<JSONUploadDataProvider> json_data_provider;
    std::shared_ptr<adapter::easy::POSTUploadAdapter> uploader;
    common_subrequest untyped_common_subreq;

    template<class Model, class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void serialize_query_params_impl(const tag::query<Model, SpecificQueryParams...> &q,
                                     Tracer tracer = Tracer())
    {
        typename tag::query<Model, SpecificQueryParams...>::aggregator_serializer_type serializer;
        {
            auto acc = json_data_provider->access();
            q.template format_serialize(serializer, tracer);
            serializer. template finalize(acc.object(), tracer);
        }
    }

    template<class Model, class ...SortParams, class Tracer = txml::EmptyTracer>
    void serialize_sort_params_impl(const tag::sort<Model, SortParams...> &t,
                                    Tracer tracer = Tracer())
    {
        SortSubrequestToJSON<Model, SortParams...> serializer;
        t.instance.template format_serialize(serializer, tracer);
        {
            auto acc = json_data_provider->access();
            serializer.finalize(acc.object(), tracer);
        }
    }

    // private execute_impl
    template<class Tracer = txml::EmptyTracer>
    void execute_impl(const std::string& index_name, size_t amount,
                 bool curl_verbose = false, Tracer tracer = Tracer());


    template <class Rep, class Period = std::ratio<1>, class Tracer = txml::EmptyTracer>
    void execute_impl(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        execute_impl(index_name, amount, ::model::KeepAlive {keep_alive}, curl_verbose, tracer);
    }

    template<class Tracer = txml::EmptyTracer>
    void execute_impl(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 bool curl_verbose = false, Tracer tracer = Tracer());

    template<class Tracer = txml::EmptyTracer>
    void execute_impl(size_t amount, const ::model::PIT &pit,
                 bool curl_verbose = false, Tracer tracer = Tracer());

};
} // namespace search
} // namespace v7
} // namespace elasticsearch

#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_NEW_HPP
