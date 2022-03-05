#ifndef ANSWER_MODEL_SEARCH_OBJECT_SORT_ORDER_H
#define ANSWER_MODEL_SEARCH_OBJECT_SORT_ORDER_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Order : public txml::XMLNodeLeaf<Order, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Order, std::string>;

    static constexpr std::string_view class_name()
    {
        return "order";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Order(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_SORT_ORDER_H
