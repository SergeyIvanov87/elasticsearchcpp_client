#ifndef ANSWER_MODEL_SEARCH_OBJECT_TIMESTAMP_H
#define ANSWER_MODEL_SEARCH_OBJECT_TIMESTAMP_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/search/object/sort/Format.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/Order.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/NumericType.h"
#include "elasticsearch/v7_10/answer_model/search/object/sort/UnmappedType.h"
#include "elasticsearch/utils/strings.hpp"

namespace model
{
template<class Model, class Element>
class SortRecord: public txml::XMLNode<SortRecord<Model, Element>, Order, UnmappedType>
{
public:
    using base_t = txml::XMLNode<SortRecord<Model, Element>, Order, UnmappedType>;

    static constexpr std::string_view element_to_search = Model::template get_path<Element>();
    static constexpr std::string_view suffix = std::string_view(".keyword");
    static constexpr std::string_view class_name()
    {

        return join_v<element_to_search, suffix>;
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    SortRecord() = default;

    SortRecord(Order&& value)
    {
        this->template emplace<Order>(std::move(value));
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_TIMESTAMP_H
