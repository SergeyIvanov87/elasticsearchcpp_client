#ifndef BOOK_DATA_MODEL_SERIALIZER_H
#define BOOK_DATA_MODEL_SERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/books/data_model/model.hpp"

namespace elasticsearch
{
namespace book
{
using namespace json;
template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(to_data, Parent, ToJSON,
                                           model::data, BOOK_DATA_MODEL_ELEMENTS)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
};
} // namespace book
} // namespace elasticsearch
#endif // BOOK_DATA_MODEL_SERIALIZER_H
