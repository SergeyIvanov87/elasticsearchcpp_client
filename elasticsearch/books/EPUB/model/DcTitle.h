#ifndef BOOK_EPUB_DC_TITLE_H
#define BOOK_EPUB_DC_TITLE_H

#include <txml/txml_fwd.h>
#include "elasticsearch/books/EPUB/model/TextElement.h"

namespace EPUB
{
class DcTitle: public txml::XMLNode<DcTitle, TextElement>
{
public:
    using base_t = txml::XMLNode<DcTitle, TextElement>;

    static constexpr std::string_view class_name()
    {
        return "dc:title";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // BOOK_EPUB_DC_TITLE_H
