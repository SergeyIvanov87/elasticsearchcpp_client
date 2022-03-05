#ifndef BOOK_DATA_MODEL_DESERIALIZER_H
#define BOOK_DATA_MODEL_DESERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/books/data_model/model.hpp"


namespace elasticsearch
{
namespace book
{
namespace model
{
using namespace json;
template<class ParentAggregator>
TXML_PREPARE_DESERIALIZER_DISPATCHABLE_CLASS(from_data, ParentAggregator, FromJSON,
                                             data, BOOK_DATA_MODEL_ELEMENTS)
{
    TXML_DESERIALIZER_DISPATCHABLE_OBJECT
};
} // namespace model
} // namespace book
} // namespace elasticsearch
#endif // BOOK_DATA_MODEL_DESERIALIZER_H
