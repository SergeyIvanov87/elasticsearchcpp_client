#ifndef ANSWER_MODEL_SEARCH_BOOLEAN_NEW_BOOL_H
#define ANSWER_MODEL_SEARCH_BOOLEAN_NEW_BOOL_H

#include "elasticsearch/v7_10/answer_model/search/object/boolean/must/elements.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/filter/elements.hpp"

namespace model
{
namespace search
{
template<class Model, class ...SubContexts>
class Boolean : public txml::XMLNode<Boolean<Model, SubContexts...>,
                                        SubContexts...>, //Must, Filter,
                   public TagHolder<QueryElementTag>
{
public:
    using self_t = Boolean<Model, SubContexts...>;
    using base_t = txml::XMLNode<Boolean<Model, SubContexts...>,
                                 SubContexts...>;

    static constexpr std::string_view class_name()
    {
        return "bool";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }


    Boolean(const Boolean &src) {
        this->getValue() = src.getValue();
    }

    Boolean(Boolean &&src) {
        this->getValue().swap(src.getValue());
    }

    template<class ...BooleanParamsTagsPack,
                        class = std::enable_if_t<
                                                not std::disjunction_v<
                                                            std::is_same<std::decay_t<BooleanParamsTagsPack>, Boolean>...
                                                                      >
                                                        , int>>
    Boolean(BooleanParamsTagsPack &&...args)
    {
        (this->template set<std::decay_t<BooleanParamsTagsPack>>(std::make_shared<std::decay_t<BooleanParamsTagsPack>>(std::forward<BooleanParamsTagsPack>(args))),...);
    }

    template<class Parent>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, Parent, ToJSON,
                                               Boolean<Model, SubContexts...>,
                                                        SubContexts...) {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>,
                                             typename SubContexts::subcontext_serializer_type<aggregator_serializer_type>...)
    {
        TXML_SERIALIZER_AGGREGATOR_OBJECT
    };

    template<class Parent>
    using subcontext_serializer_type = serializer_parted_type<Parent>;

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        aggregator_serializer_type ser(out.get_shared_mediator_object());
        base_t:: template format_serialize_impl(ser, tracer);
        //*(out.get_shared_mediator_object()) = ser. template finalize(tracer);
    }
};
} // namespace search
} // namespace model
#endif
