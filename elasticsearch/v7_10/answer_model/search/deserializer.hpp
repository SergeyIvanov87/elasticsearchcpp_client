#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_DESERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_DESERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/search/models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
using namespace json;
template<class SpecificModel, class Parent>
TXML_PREPARE_DESERIALIZER_DISPATCHABLE_CLASS(ResponseFromJSON, Parent,
                                                  FromJSON,
                                                            response<SpecificModel>,
                                                                     ::model::TimedOut,
                                                                     ::model::Took,
                                                                     ::model::_Shards,
                                                                        ::model::Total, ::model::Successful, ::model::Failed,
                                                                     ::model::HitsNode<SpecificModel>,
                                                                        ::model::HitsTotal, ::model::MaxScore, ::model::HitsArray<SpecificModel>,
                                                                            ::model::Value, ::model::Relation, typename ::model::HitsArray<SpecificModel>::element_t,
                                                                            ::model::_Index, ::model::_Id, ::model::_Score, ::model::_Type, ::model::_Source<SpecificModel>,
                                                                     ::model::PitID,
                                                                     Error,
                                                                        RootCause,
                                                                            typename RootCause::element_t,
                                                                        ::model::Type,
                                                                        ::model::Reason,
                                                                        ::model::CausedBy,
                                                                        ::model::Status)
{
    TXML_DESERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;

    template<class Tracer>
    std::shared_ptr<response<SpecificModel>> deserialize_impl(txml::details::SchemaDTag<response<SpecificModel>>, Tracer tracer)
    {
        auto& [begin_it, end_it] = this->get_shared_mediator_object()->top();

        auto ret = this->template create_deserialized_node<response<SpecificModel>>(tracer, std::distance(begin_it, end_it));
        ++begin_it;

        return ret;
    }

    template<class Tracer>
    std::shared_ptr<typename ::model::HitsArray<SpecificModel>::element_t> deserialize_impl(txml::details::SchemaDTag< typename ::model::HitsArray<SpecificModel>::element_t>, Tracer tracer)
    {
        auto mediator = this->get_shared_mediator_object();
        auto& [begin_it, end_it] = mediator->top();
        if (!this-> template check_array_node_param< typename ::model::HitsArray<SpecificModel>::element_t>(begin_it, end_it, json::value_t::object, tracer))
        {
            return {};
        }

        mediator->emplace(begin_it.value().begin(), begin_it.value().end());
        auto ret = this->template create_deserialized_node< typename ::model::HitsArray<SpecificModel>::element_t>(tracer, std::distance(begin_it.value().begin(), begin_it.value().end()));
        mediator->pop();
        ++begin_it;

        return ret;
    }

    template<class Tracer>
    std::shared_ptr<typename RootCause::element_t> deserialize_impl(txml::details::SchemaDTag< typename RootCause::element_t>, Tracer tracer)
    {
        auto mediator = this->get_shared_mediator_object();
        auto& [begin_it, end_it] = mediator->top();
        if (!this-> template check_array_node_param< typename RootCause::element_t>(begin_it, end_it, json::value_t::object, tracer))
        {
            return {};
        }

        mediator->emplace(begin_it.value().begin(), begin_it.value().end());
        auto ret = this-> template create_deserialized_node< typename RootCause::element_t>(tracer, std::distance(begin_it.value().begin(), begin_it.value().end()));
        mediator->pop();
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
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_DESERIALIZER_HPP
