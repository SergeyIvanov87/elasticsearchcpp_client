#ifndef ELASTICSEARCH_AUX_DOC_INDEX_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_AUX_DOC_INDEX_MODEL_SERIALIZER_HPP

#include <txml/applications/json/json.hpp>
#include "elasticsearch/service/doc_id/model.hpp"

namespace elasticsearch
{
namespace aux
{
namespace model
{
using namespace json;
template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(to_data, Parent, ToJSON, AUX_DATA_MODEL_ELEMENTS)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;
};
}
}
}
#endif // ELASTICSEARCH_AUX_DOC_INDEX_MODEL_SERIALIZER_HPP
