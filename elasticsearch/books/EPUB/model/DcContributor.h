#ifndef BOOK_EPUB_DC_CONTRIBUTOR_H
#define BOOK_EPUB_DC_CONTRIBUTOR_H

#include <txml/txml_fwd.h>
#include "elasticsearch/books/EPUB/model/TextElement.h"

namespace EPUB
{
class DcContributor: public txml::XMLNode<DcContributor, TextElement>
{
public:
    using base_t = txml::XMLNode<DcContributor, TextElement>;

    static constexpr std::string_view class_name()
    {
        return "dc:contributor";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // BOOK_EPUB_DC_CONTRIBUTOR_H
