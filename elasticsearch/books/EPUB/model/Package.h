#ifndef BOOK_EPUB_PACKAGE_H
#define BOOK_EPUB_PACKAGE_H

#include <txml/txml_fwd.h>

#include "elasticsearch/books/EPUB/model/Metadata.h"

namespace EPUB
{
class Package: public txml::XMLNode<Package, Metadata>
{
public:
    using base_t = txml::XMLNode<Package, Metadata>;

    static constexpr std::string_view class_name()
    {
        return "package";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

using OPF = Package;
}
#endif // BOOK_EPUB_PACKAGE_H
