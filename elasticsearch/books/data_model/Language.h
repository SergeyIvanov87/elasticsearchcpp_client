#ifndef BOOK_DATA_MODEL_LANGUAGE_H
#define BOOK_DATA_MODEL_LANGUAGE_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace element
{
class Language: public txml::XMLNodeLeaf<Language, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Language, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "language";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Language(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Language(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // BOOK_DATA_MODEL_LANGUAGE_H
