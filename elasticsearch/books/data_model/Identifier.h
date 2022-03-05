#ifndef BOOK_DATA_MODEL_IDENTIFIER_H
#define BOOK_DATA_MODEL_IDENTIFIER_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace element
{
class Identifier: public txml::XMLNodeLeaf<Identifier, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Identifier, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "identifier";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Identifier(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Identifier(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // BOOK_DATA_MODEL_IDENTIFIER_H
