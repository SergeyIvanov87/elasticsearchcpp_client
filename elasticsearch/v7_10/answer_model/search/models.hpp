#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_HPP

#include "elasticsearch/v7_10/answer_model/pit/object/Pit.h"
#include "elasticsearch/v7_10/answer_model/object/Size.h"
#include "elasticsearch/v7_10/answer_model/search/object/Query.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/sort_array.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/UnmappedType.h"

#include "elasticsearch/v7_10/answer_model/search/object/resp/HitsNode.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/TimedOut.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/Took.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/PitID.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Shards.h"

//todo put up common header with types list!!!
#include "elasticsearch/v7_10/answer_model/object/Error.h"
#include "elasticsearch/v7_10/answer_model/object/CausedBy.h"
#include "elasticsearch/v7_10/answer_model/object/RootCause.h"
#include "elasticsearch/v7_10/answer_model/object/Status.h"

namespace elasticsearch
{
namespace v7
{
namespace search
{
/*
 * It is impossible to write down `request` in fully templated manner.
 * So let's take consideration to write down multiple subrequests which can be highly template customizable by itself
 * */
template<class Model, class ...SpecificSortParams>
class sort_subrequest : public txml::XMLNode<sort_subrequest<Model, SpecificSortParams...>,
                                            ::model::SortArray<Model, SpecificSortParams...>>
{
public:
    using base = txml::XMLNode<sort_subrequest<Model, SpecificSortParams...>,
                               ::model::SortArray<Model, SpecificSortParams...>>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

class common_subrequest : public txml::XMLNode<common_subrequest,
                                               ::model::PIT,
                                               ::model::Size>
{
public:
    using base = txml::XMLNode<common_subrequest, ::model::PIT, ::model::Size>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO put specicid search elements in 'search/Error.h' additinal file
using RootCause = ::model::RootCause<>;
using Error = ::model::Error<RootCause, ::model::CausedBy>;

template<class SpecificModel>
class response: public txml::XMLNode<response<SpecificModel>,
                                     ::model::TimedOut,
                                     ::model::Took,
                                     ::model::_Shards,
                                     ::model::HitsNode<SpecificModel>,
                                     ::model::PitID,
                                     Error,
                                        RootCause,
                                            typename RootCause::element_t,
                                        ::model::Type,
                                        ::model::Reason,
                                        ::model::CausedBy,
                                     ::model::Status>
{
public:
    using base = txml::XMLNode<response<SpecificModel>,
                               ::model::TimedOut,
                               ::model::Took,
                               ::model::_Shards,
                               ::model::HitsNode<SpecificModel>,
                               ::model::PitID,
                               Error,
                                        RootCause,
                                            typename RootCause::element_t,
                                        ::model::Type,
                                        ::model::Reason,
                                        ::model::CausedBy,
                               ::model::Status>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_HPP
