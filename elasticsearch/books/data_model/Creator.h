#ifndef BOOK_DATA_MODEL_CREATOR_H
#define BOOK_DATA_MODEL_CREATOR_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace element
{
class Creator: public txml::XMLNodeLeaf<Creator, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Creator, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "creator";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Creator(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Creator(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // BOOK_DATA_MODEL_CREATOR_H
