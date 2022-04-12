#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_HPP

#include "elasticsearch/v7_10/answer_model/pit/object/Pit.h"
#include "elasticsearch/v7_10/answer_model/object/Size.h"
#include "elasticsearch/v7_10/answer_model/search/object/Query.h"
#include "elasticsearch/v7_10/answer_model/search/object/new_Query.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/sort_array.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/UnmappedType.h"

#include "elasticsearch/v7_10/answer_model/search/object/resp/HitsNode.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/TimedOut.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/Took.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/PitID.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Shards.h"

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
#if 0
template<class Model, class ...SpecificQueryParams>
class query_subrequest : public txml::XMLNode<query_subrequest<Model, SpecificQueryParams...>,
                                              ::model::Query<Model, SpecificQueryParams...>>
{
public:
    using base = txml::XMLNode<query_subrequest<Model, SpecificQueryParams...>,
                               ::model::Query<Model, SpecificQueryParams...>>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
#endif
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

template<class SpecificModel>
class response: public txml::XMLNode<response<SpecificModel>,
                                     ::model::TimedOut,
                                     ::model::Took,
                                     ::model::_Shards,
                                     ::model::HitsNode<SpecificModel>,
                                     ::model::PitID>
{
public:
    using base = txml::XMLNode<response<SpecificModel>,
                               ::model::TimedOut,
                               ::model::Took,
                               ::model::_Shards,
                               ::model::HitsNode<SpecificModel>,
                               ::model::PitID>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};



/////////////////////////
using namespace json;
template<class Model, class ...SpecificQueryParams>
class query_subrequest_new : public txml::XMLNode<query_subrequest_new<Model, SpecificQueryParams...>,
                                              ::model::search::QueryNew<Model, SpecificQueryParams...>>
{
public:
    using base_t = txml::XMLNode<query_subrequest_new<Model, SpecificQueryParams...>,
                               ::model::search::QueryNew<Model, SpecificQueryParams...>>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    using query_element_value_type = ::model::search::QueryNew<Model, SpecificQueryParams...>;

    // Only itself serializer must be here... just relief
    //         a) we must declate serialzier for element by itself
    template <class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                                    query_subrequest_new<Model, SpecificQueryParams...>,
                                                    ::model::search::QueryNew<Model, SpecificQueryParams...>)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const query_subrequest_new<Model, SpecificQueryParams...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skip query_subrequest by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename SpecificQueryParams::subcontext_serializer_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_impl(ser, tracer);
    }
};
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_SEARCH_MODEL_HPP
