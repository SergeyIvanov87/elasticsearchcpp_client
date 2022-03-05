#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_DELETE_MODEL_DESERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_DELETE_MODEL_DESERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/delete/model.hpp"

namespace elasticsearch
{
namespace v7
{
namespace delete_data
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
} // namespace delete_data
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_DELETE_MODEL_DESERIALIZER_HPP
