#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_PUT_DATA_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_PUT_DATA_MODEL_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>

#include "elasticsearch/v7_10/answer_model/data_creation/models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace put_json_data
{
using namespace json;
/*
template<class SpecificModel, template<typename> class ...SpecificModelSerializer>
TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(RequestSerializer, SpecificModelSerializer<RequestSerializer<SpecificModel, SpecificModelSerializer...>>...)
{
    TXML_SERIALIZER_AGGREGATOR_OBJECT
}*/

template<class SpecificModel, template<typename> class ...SpecificModelSerializer>
struct RequestSerializer : public txml::SerializerVirtualDispatcher<SerializerCore, SpecificModelSerializer<RequestSerializer<SpecificModel,
                                                                                                             SpecificModelSerializer...>>...>
{
    using base_t = txml::SerializerVirtualDispatcher<SerializerCore, SpecificModelSerializer<RequestSerializer<SpecificModel,
                                                                              SpecificModelSerializer...>>...>;
//    using json = nlohmann::json;
    RequestSerializer(std::shared_ptr<std::stack<json::SerializerCore::json_core_t>> external_iterators_stack =
                            std::shared_ptr<std::stack<json::SerializerCore::json_core_t>>(new std::stack<json::SerializerCore::json_core_t>)) :
        base_t(external_iterators_stack)
    {
    }
};
} // namespace put_json_data
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_DESERIALIZER_HPP
