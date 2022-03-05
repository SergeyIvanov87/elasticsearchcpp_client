#ifndef BOOK_EPUB_DC_CREATOR_H
#define BOOK_EPUB_DC_CREATOR_H

#include <txml/txml_fwd.h>
#include "elasticsearch/books/EPUB/model/TextElement.h"


namespace EPUB
{
class DcCreator: public txml::XMLNode<DcCreator, TextElement>
{
public:
    using base_t = txml::XMLNode<DcCreator, TextElement>;

    static constexpr std::string_view class_name()
    {
        return "dc:creator";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // BOOK_EPUB_DC_CREATOR_H
