#ifndef ANSWER_MODEL_SEARCH_OBJECT_SORT_H
#define ANSWER_MODEL_SEARCH_OBJECT_SORT_H

#include "elasticsearch/v7_10/answer_model/search/object/sort/sort_element.h"

namespace model
{
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
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_SORT_H
