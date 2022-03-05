#ifndef BOOK_DATA_MODEL_CONTRIBUTOR_H
#define BOOK_DATA_MODEL_CONTRIBUTOR_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace element
{
class Contributor: public txml::XMLNodeLeaf<Contributor, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Contributor, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "contributor";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Contributor(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Contributor(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // BOOK_DATA_MODEL_CONTRIBUTOR_H
