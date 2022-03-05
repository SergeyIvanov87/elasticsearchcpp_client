#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_DELETE_PIT_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_DELETE_PIT_MODEL_HPP

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/object/Succeeded.h"
#include "elasticsearch/v7_10/answer_model/object/NumFreed.h"
#include "elasticsearch/v7_10/answer_model/object/CausedBy.h"
#include "elasticsearch/v7_10/answer_model/object/Error.h"
#include "elasticsearch/v7_10/answer_model/object/Status.h"

namespace elasticsearch
{
namespace v7
{
namespace delete_pit
{
using RootCause = ::model::RootCause<>;
using Error = ::model::Error<RootCause, ::model::CausedBy>;

class response : public txml::XMLNode<response, ::model::Succeeded, ::model::NumFreed,
                                                Error, ::model::Status>
{
public:
    using base = txml::XMLNode<response, ::model::Succeeded, ::model::NumFreed,
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
} // namespace delete_pit
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_DELETE_PIT_MODEL_HPP
