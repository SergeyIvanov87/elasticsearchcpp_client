#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_DATA_CREATION_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_DATA_CREATION_MODEL_HPP
/*
#include <memory>
#include <sstream>
#include "adapter/easy/sstream_receiver.hpp"
*/
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Id.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Index.h"
#include "elasticsearch/v7_10/answer_model/object/_Type.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Version.h"
#include "elasticsearch/v7_10/answer_model/object/Result.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Shards.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_SeqNo.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_PrimaryTerm.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/Found.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Source.h"

namespace elasticsearch
{
namespace v7
{
namespace put_json_data
{
class response : public txml::XMLNode<response, ::model::_Id, ::model::_Index, ::model::_Type,
                                                ::model::_Version, ::model::Result,
                                                ::model::_Shards,
                                                ::model::_SeqNo,
                                                ::model::_PrimaryTerm>
{
public:
    using base = txml::XMLNode<response, ::model::_Id, ::model::_Index, ::model::_Type,
                                         ::model::_Version, ::model::Result,
                                         ::model::_Shards,
                                         ::model::_SeqNo,
                                         ::model::_PrimaryTerm>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace put_json_data

namespace get_json_data
{
template<class SpecificMappingModel>
class response : public txml::XMLNode<response<SpecificMappingModel>,
                                                ::model::_Id, ::model::_Index, ::model::_Type,
                                                ::model::_Version,
                                                ::model::_SeqNo,
                                                ::model::_PrimaryTerm,

                                                ::model::Found,
                                                ::model::_Source<SpecificMappingModel>>
{
public:
    using base = txml::XMLNode<response<SpecificMappingModel>,
                                         ::model::_Id, ::model::_Index, ::model::_Type,
                                         ::model::_Version,
                                         ::model::_SeqNo,
                                         ::model::_PrimaryTerm,

                                         ::model::Found,
                                         ::model::_Source<SpecificMappingModel>>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace get_json_data

} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_DATA_CREATION_MODEL_HPP
