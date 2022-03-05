#ifndef COMMON_DATA_MODEL_SERIALIZER_H
#define COMMON_DATA_MODEL_SERIALIZER_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/common_model/model.hpp"

namespace elasticsearch
{
namespace common_model
{
using namespace json;
template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(to_data, Parent, ToJSON, COMMON_DATA_MODEL_ELEMENTS)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;
};
} // namespace common_model
} // namespace elasticsearch
#endif // COMMON_DATA_MODEL_SERIALIZER_H
