#ifndef BOOK_DATA_MODEL_MODEL_H
#define BOOK_DATA_MODEL_MODEL_H

#include <txml/txml_fwd.h>
#include "elasticsearch/common_model/model.hpp"
#include "elasticsearch/books/data_model/Contributor.h"
#include "elasticsearch/books/data_model/Creator.h"
#include "elasticsearch/books/data_model/Identifier.h"
#include "elasticsearch/books/data_model/Language.h"
#include "elasticsearch/books/data_model/Title.h"

namespace elasticsearch
{
namespace book
{
namespace model
{
#define BOOK_DATA_MODEL_ELEMENTS        elasticsearch::book::model::element::Contributor,          \
                                        elasticsearch::book::model::element::Creator,              \
                                        elasticsearch::book::model::element::Identifier,           \
                                        elasticsearch::book::model::element::Language,             \
                                        elasticsearch::book::model::element::Title

struct data : public txml::XMLNode<data,
                                   BOOK_DATA_MODEL_ELEMENTS,
                                   COMMON_DATA_MODEL_ELEMENTS>
{
    using base = txml::XMLNode<data,
                               BOOK_DATA_MODEL_ELEMENTS,
                               COMMON_DATA_MODEL_ELEMENTS>;

    static constexpr std::string_view class_name()
    {
        return "BookModel";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
}
}
#endif // BOOK_DATA_MODEL_MODEL_H
