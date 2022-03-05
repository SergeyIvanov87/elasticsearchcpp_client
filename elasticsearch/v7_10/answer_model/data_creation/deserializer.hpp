#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_DATA_CREATION_MODEL_DESERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_DATA_CREATION_MODEL_DESERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/data_creation/models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace put_json_data
{
using namespace json;
TXML_DECLARE_DESERIALIZER_CLASS(ResponseFromJSON, FromJSON, response,
                                                                ::model::_Id,
                                                                ::model::_Index,
                                                                ::model::_Type,
                                                                ::model::_Version,
                                                                ::model::Result,
                                                                ::model::_Shards,
                                                                    ::model::Failed,
                                                                    ::model::Successful,
                                                                    ::model::Total,
                                                                ::model::_SeqNo,
                                                                ::model::_PrimaryTerm)
{
    TXML_DESERIALIZER_OBJECT

    template<class Tracer>
    std::shared_ptr<response> deserialize_impl(txml::details::SchemaDTag<response>, Tracer tracer)
    {
        auto& [begin_it, end_it] = get_shared_mediator_object()->top();

        auto ret = create_deserialized_node<response>(tracer, std::distance(begin_it.value().begin(), begin_it.value().end()));
        ++begin_it;

        return ret;
    }
};
} // namespace put_json_data


namespace get_json_data
{
using namespace json;
template<class SpecificModel, class Parent>
TXML_PREPARE_DESERIALIZER_DISPATCHABLE_CLASS(ResponseFromJSON, Parent,
                                                  FromJSON,
                                                            response<SpecificModel>,
                                                            ::model::_Id, ::model::_Index, ::model::_Type,
                                                            ::model::_Version, ::model::Result,
                                                            ::model::_Shards,
                                                            ::model::_SeqNo,
                                                            ::model::_PrimaryTerm,

                                                            ::model::Found,
                                                            ::model::_Source<SpecificModel>)
{
    TXML_DESERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    std::shared_ptr<response<SpecificModel>> deserialize_impl(txml::details::SchemaDTag<response<SpecificModel>>, Tracer tracer)
    {
        auto& [begin_it, end_it] = this->get_shared_mediator_object()->top();

        auto ret = this->template create_deserialized_node<response<SpecificModel>>(tracer, std::distance(begin_it.value().begin(), begin_it.value().end()));
        ++begin_it;

        return ret;
    }

};




template<class SpecificModel, template<typename> class ...SpecificModelSerializer>
struct ResponseDeSerializer : public txml::DeserializerDispatcher<ResponseFromJSON<SpecificModel, ResponseDeSerializer<SpecificModel, SpecificModelSerializer...>>,
                                                      SpecificModelSerializer<ResponseDeSerializer<SpecificModel, SpecificModelSerializer...>>...>
{
    using base_t = txml::DeserializerDispatcher<ResponseFromJSON<SpecificModel, ResponseDeSerializer<SpecificModel, SpecificModelSerializer...>>,
                                    SpecificModelSerializer<ResponseDeSerializer<SpecificModel, SpecificModelSerializer...>>...>;
    using json = nlohmann::json;
    ResponseDeSerializer(nlohmann::json &obj, std::shared_ptr<std::stack<DeserializerCore::range_iterator>> shared_iterators_stack =
                                        std::shared_ptr<std::stack<DeserializerCore::range_iterator>>(new std::stack<DeserializerCore::range_iterator>)) :
        base_t(obj, shared_iterators_stack)
    {
    }
};
} // namespace get_json_data
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_DATA_CREATION_MODEL_DESERIALIZER_HPP
