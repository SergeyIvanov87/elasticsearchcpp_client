#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_HPP

#include <memory>
#include <sstream>
#include "adapter/easy/sstream_receiver.hpp"

#include "elasticsearch/v7_10/answer_model/object/Ack.h"
#include "elasticsearch/v7_10/answer_model/object/ShardsAck.h"
#include "elasticsearch/v7_10/answer_model/object/Index.h"
#include "elasticsearch/v7_10/answer_model/object/IndexUUID.h"
#include "elasticsearch/v7_10/answer_model/object/Error.h"
#include "elasticsearch/v7_10/answer_model/object/Status.h"

#include "elasticsearch/v7_10/answer_model/index_mapping/object/Mappings.h"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping
{

template<class SpecificMappingModel>
struct request : public txml::XMLNode<request<SpecificMappingModel>, ::model::Mappings<SpecificMappingModel>>
{
    using base = txml::XMLNode<request<SpecificMappingModel>, ::model::Mappings<SpecificMappingModel>>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


using RootCause = ::model::RootCause<::model::Index, ::model::IndexUUID>;
using Error = ::model::Error<RootCause, ::model::Index, ::model::IndexUUID>;

class response : public txml::XMLNode<response, ::model::Ack, ::model::ShardsAck, ::model::Index,
                                                Error, ::model::Status>
{
public:
    using base = txml::XMLNode<response, ::model::Ack, ::model::ShardsAck, ::model::Index,
                                         Error, ::model::Status>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace index_mapping
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_MODEL_HPP
