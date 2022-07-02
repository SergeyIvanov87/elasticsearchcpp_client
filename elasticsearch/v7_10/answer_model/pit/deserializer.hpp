#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_PIT_MODEL_DESERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_PIT_MODEL_DESERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/pit/delete_pit_model.hpp"

namespace elasticsearch
{
namespace v7
{
namespace delete_pit
{
using namespace json;
TXML_DECLARE_DESERIALIZER_CLASS(ResponseFromJSON, FromJSON, response,
                                                                        ::model::Succeeded,
                                                                        ::model::NumFreed,
                                                                        Error,
                                                                                RootCause,
                                                                                    typename RootCause::element_t,
                                                                                ::model::Type,
                                                                                ::model::Reason,
                                                                                ::model::CausedBy,
                                                                        ::model::Status)
{
    TXML_DESERIALIZER_OBJECT

    template<class Tracer>
    std::optional<response> deserialize_impl(txml::details::SchemaDTag<response>, Tracer tracer)
    {
        auto& [begin_it, end_it] = get_shared_mediator_object()->top();

        auto ret = create_deserialized_node<response>(tracer, std::distance(begin_it, end_it));
        ++begin_it;

        return ret;
    }

    template<class Tracer>
    std::optional<typename RootCause::element_t> deserialize_impl(txml::details::SchemaDTag< typename RootCause::element_t>, Tracer tracer)
    {
        auto mediator = get_shared_mediator_object();
        auto& [begin_it, end_it] = mediator->top();
        if (!check_array_node_param< typename RootCause::element_t>(begin_it, end_it, in_stream_core_t::value_t::object, tracer))
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
} // namespace delete_pit
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_PIT_MODEL_DESERIALIZER_HPP
