#ifndef COMMON_DATA_MODEL_DESERIALIZER_H
#define COMMON_DATA_MODEL_DESERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/common_model/model.hpp"

namespace elasticsearch
{
namespace common_model
{
using namespace json;
template<class ParentAggregator>
TXML_PREPARE_DESERIALIZER_DISPATCHABLE_CLASS(from_data, ParentAggregator,
                                                      FromJSON, COMMON_DATA_MODEL_ELEMENTS)
{
    TXML_DESERIALIZER_DISPATCHABLE_OBJECT
};
} // namespace common_model
} // namespace elasticsearch
#endif // COMMON_DATA_MODEL_DESERIALIZER_H
