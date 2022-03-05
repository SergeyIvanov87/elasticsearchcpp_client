#ifndef BOOK_FB2_SHORTFICTIONBOOK_H
#define BOOK_FB2_SHORTFICTIONBOOK_H

#include <txml/txml_fwd.h>

namespace fb2
{
class ShortFictionBook : public txml::XMLNode<ShortFictionBook, Description>
{
public:
    using base = txml::XMLNode<ShortFictionBook, Description>;

    static constexpr std::string_view class_name()
    {
        return "FictionBook";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
} // namespace fb2
#endif // BOOK_FB2_SHORTFICTIONBOOK_H
