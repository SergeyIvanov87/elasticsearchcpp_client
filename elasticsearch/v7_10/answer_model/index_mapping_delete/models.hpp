#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_DELETE_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_DELETE_MODEL_HPP

#include <memory>
#include <sstream>
#include "adapter/easy/sstream_receiver.hpp"

#include "elasticsearch/v7_10/answer_model/object/Ack.h"
#include "elasticsearch/v7_10/answer_model/object/ResourceId.h"
#include "elasticsearch/v7_10/answer_model/object/ResourceType.h"
#include "elasticsearch/v7_10/answer_model/object/Index.h"
#include "elasticsearch/v7_10/answer_model/object/IndexUUID.h"
#include "elasticsearch/v7_10/answer_model/object/Error.h"
#include "elasticsearch/v7_10/answer_model/object/Status.h"

namespace elasticsearch
{
namespace v7
{
namespace index_mapping_delete
{

using RootCause = ::model::RootCause<::model::Index, ::model::IndexUUID, ::model::ResourceType, ::model::ResourceId>;
using Error = ::model::Error<RootCause, ::model::Index, ::model::IndexUUID, ::model::ResourceType, ::model::ResourceId>;

class response : public txml::XMLNode<response, ::model::Ack, Error, ::model::Status>
{
public:
    using base = txml::XMLNode<response, ::model::Ack, Error, ::model::Status>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace index_mapping_delete
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_DELETE_MODEL_HPP
