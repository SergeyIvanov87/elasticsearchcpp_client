#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_UPDATE_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_UPDATE_MODEL_HPP

#include "elasticsearch/v7_10/answer_model/index_mapping/index_mapping_models.hpp"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping_update
{

template<class SpecificMappingModel>
struct request : public txml::XMLNode<request<SpecificMappingModel>, ::model::Properties<SpecificMappingModel>>
{
    using base = txml::XMLNode<request<SpecificMappingModel>, ::model::Properties<SpecificMappingModel>>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

using response = index_mapping::response; ??? Ack only!!!!
} // namespace index_mapping_update
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_UPDATE_MODEL_HPP
