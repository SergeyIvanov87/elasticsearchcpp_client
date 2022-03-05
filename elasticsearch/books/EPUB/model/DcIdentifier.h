#ifndef BOOK_EPUB_DC_IDENTIFIER_H
#define BOOK_EPUB_DC_IDENTIFIER_H

#include <txml/txml_fwd.h>
#include "elasticsearch/books/EPUB/model/TextElement.h"

namespace EPUB
{
class DcIdentifier: public txml::XMLNode<DcIdentifier, TextElement>
{
public:
    using base_t = txml::XMLNode<DcIdentifier, TextElement>;

    static constexpr std::string_view class_name()
    {
        return "dc:identifier";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // BOOK_EPUB_DC_IDENTIFIER_H
