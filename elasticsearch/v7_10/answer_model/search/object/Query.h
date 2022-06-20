#ifndef ANSWER_MODEL_SEARCH_OBJECT_QUERY_H
#define ANSWER_MODEL_SEARCH_OBJECT_QUERY_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/search/object/MatchAll.h"

namespace model
{
namespace search
{
using namespace json;
template<class Model, class ...Params>
class Query: public txml::XMLNode<Query<Model, Params...>,
                                  Params...>
{
public:
    using base_t = txml::XMLNode<Query<Model, Params...>,
                                 Params...>;
    using base_t::base_t;
    static constexpr std::string_view class_name()
    {
        return "query";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
/*
    template<class ...SpecificParams>
    Query(SpecificParams &&...args)
    {
        (this->template emplace<std::decay_t<SpecificParams>>(std::forward<SpecificParams>(args)),...);
    }
*/
    template<class ParentAggregator>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, ParentAggregator, ToJSON,
                                                    Query<Model, Params...>,
                                                    Params...)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename Params::serializer_parted_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_request(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_request(ser, tracer);
    }
};
using namespace json;
template<>
class Query<EmptyModel, EmptyParam>: public txml::XMLNode<Query<EmptyModel, EmptyParam>,
                                                          MatchAll>
{
public:
    using base_t = txml::XMLNode<Query<EmptyModel, EmptyParam>,
                                       MatchAll>;

    static constexpr std::string_view class_name()
    {
        return "query";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
    Query() {
        this->template emplace<MatchAll>();
    }

    template<class ParentAggregator>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, ParentAggregator, ToJSON,
                                                    Query<EmptyModel, EmptyParam>,
                                                    MatchAll)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const MatchAll&)
        {
            auto mediator = this->get_shared_mediator_object();
            typename base_t::json_core_t element({{MatchAll::class_name(), ""}});
            mediator->push(std::move(element));
        }
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Formatter, class Tracer>
    void format_serialize_request(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_request(ser, tracer);
    }
};
}
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_QUERY_H
