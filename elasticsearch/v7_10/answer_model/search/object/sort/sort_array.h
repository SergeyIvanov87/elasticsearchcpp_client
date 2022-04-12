#ifndef ANSWER_MODEL_SEARCH_OBJECT_SORT_H
#define ANSWER_MODEL_SEARCH_OBJECT_SORT_H

#include "elasticsearch/v7_10/answer_model/search/object/sort/sort_element.h"

namespace model
{
using namespace json;
template<class Model, class ...SortedElements>
class SortArrayElement : public txml::XMLNode<SortArrayElement<Model, SortedElements...>,
                                              SortRecord<Model, SortedElements>...>
{
public:
    using base_t = txml::XMLNode<SortArrayElement<Model, SortedElements...>,
                                 SortRecord<Model, SortedElements>...>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};


template<class Model, class ...SortedElements>
class SortArray: public txml::XMLArray<SortArray<Model, SortedElements...>,
                                               SortArrayElement<Model, SortedElements...>>
{
public:
    using element_t = SortArrayElement<Model, SortedElements...>;
    using base_t = txml::XMLArray<SortArray<Model, SortedElements...>,
                                  SortArrayElement<Model, SortedElements...>>;
    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "sort";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    SortArray(std::initializer_list<::model::Order> orders)
    {
        using sorted_array_value_type = ::model::SortArrayElement<Model, SortedElements...>;
        auto elem = std::make_shared<sorted_array_value_type>();

        // helper
        auto append = [order_it = orders.begin()](auto sorted_param) mutable{
            sorted_param->template emplace<::model::Order>(*order_it);
        };

        (append(elem->template emplace<::model::SortRecord<Model, SortedElements>>()), ...);
        this->getValue().push_back(std::move(elem));
    }

    template<class ParentAggregator>
    TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(serializer_parted_type, ParentAggregator, ToJSON,
                                                model::SortArray<Model, SortedElements...>,
                                                    model::SortArrayElement<Model, SortedElements...>,
                                                        model::SortRecord<Model, SortedElements>...,
                                                            model::UnmappedType,
                                                            model::Order,
                                                            model::Format)
    {
        TXML_SERIALIZER_DISPATCHABLE_OBJECT

        template<class Tracer>
        void serialize_impl(const ::model::SortArrayElement<Model, SortedElements...> &val, Tracer tracer)
        {
            tracer.trace(__FUNCTION__, " - skip SortArrayElement by itself");
            val.template format_serialize_elements(*this, tracer);
        }
    };

    TXML_DECLARE_SERIALIZER_AGGREGATOR_CLASS(aggregator_serializer_type,
                                             serializer_parted_type<aggregator_serializer_type>)
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
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_SORT_H
