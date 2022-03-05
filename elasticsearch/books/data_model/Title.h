#ifndef BOOK_DATA_MODEL_TITLE_H
#define BOOK_DATA_MODEL_TITLE_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace element
{
class Title: public txml::XMLNodeLeaf<Title, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Title, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "title";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Title(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Title(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // BOOK_DATA_MODEL_TITLE_H
