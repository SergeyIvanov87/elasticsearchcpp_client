#ifndef BOOK_EPUB_DC_LANGUAGE_H
#define BOOK_EPUB_DC_LANGUAGE_H

#include <txml/txml_fwd.h>
#include "elasticsearch/books/EPUB/model/TextElement.h"

namespace EPUB
{
class DcLanguage: public txml::XMLNode<DcLanguage, TextElement>
{
public:
    using base_t = txml::XMLNode<DcLanguage, TextElement>;

    static constexpr std::string_view class_name()
    {
        return "dc:language";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // BOOK_EPUB_DC_LANGUAGE_H
