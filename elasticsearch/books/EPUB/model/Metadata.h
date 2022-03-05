#ifndef BOOK_EPUB_METADATA_H
#define BOOK_EPUB_METADATA_H

#include <txml/txml_fwd.h>

#include "elasticsearch/books/EPUB/model/DcTitle.h"
#include "elasticsearch/books/EPUB/model/DcIdentifier.h"
#include "elasticsearch/books/EPUB/model/DcContributor.h"
#include "elasticsearch/books/EPUB/model/DcCreator.h"
#include "elasticsearch/books/EPUB/model/DcLanguage.h"

namespace EPUB
{
class Metadata: public txml::XMLNode<Metadata, DcTitle, DcIdentifier, DcContributor, DcCreator, DcLanguage>
{
public:
    using base_t = txml::XMLNode<Metadata, DcTitle>;

    static constexpr std::string_view class_name()
    {
        return "metadata";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // BOOK_EPUB_METADATA_H
