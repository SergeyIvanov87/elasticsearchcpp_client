#ifndef ANSWER_MODEL_SEARCH_OBJECT_SORT_NUMERIC_TYPE_H
#define ANSWER_MODEL_SEARCH_OBJECT_SORT_NUMERIC_TYPE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class NumericType: public txml::XMLNodeLeaf<NumericType, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<NumericType, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "numeric_type";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    NumericType(value_t&& value) :
        base_t(std::move(value))
    {
    }

    NumericType(const value_t& value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_SORT_NUMERIC_TYPE_H
