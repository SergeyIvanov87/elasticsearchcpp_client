#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_DESERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_DESERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/index_mapping/index_mapping_models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping
{
using namespace json;
TXML_DECLARE_DESERIALIZER_CLASS(ResponseFromJSON, FromJSON, response,
                                                            ::model::Ack,
                                                            ::model::ShardsAck,
                                                            ::model::Index,
                                                            Error,
                                                                RootCause,
                                                                typename RootCause::element_t,
                                                                    ::model::Type,
                                                                    ::model::Reason,
                                                                    /*::model::Index,*/
                                                                    ::model::IndexUUID,
                                                            ::model::Status)
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


    template<class Tracer>
    std::shared_ptr<typename RootCause::element_t> deserialize_impl(txml::details::SchemaDTag< typename RootCause::element_t>, Tracer tracer)
    {
        auto mediator = get_shared_mediator_object();
        auto& [begin_it, end_it] = mediator->top();
        if (!check_array_node_param< typename RootCause::element_t>(begin_it, end_it, json::value_t::object, tracer))
        {
            return {};
        }

        mediator->emplace(begin_it.value().begin(), begin_it.value().end());
        auto ret = create_deserialized_node< typename RootCause::element_t>(tracer, std::distance(begin_it.value().begin(), begin_it.value().end()));
        mediator->pop();
        ++begin_it;

        return ret;
    }
};
} // namespace index_mapping
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_DESERIALIZER_HPP
