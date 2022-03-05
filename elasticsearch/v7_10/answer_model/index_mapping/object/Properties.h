#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_OBJECTS_PROPERTIES_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_OBJECTS_PROPERTIES_HPP

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/object/RootCause.h"
#include "elasticsearch/v7_10/answer_model/object/Type.h"
#include "elasticsearch/v7_10/answer_model/object/Reason.h"

namespace model
{
template<class SpecificMappingModel>
class Properties: public txml::XMLNode<Properties<SpecificMappingModel>, SpecificMappingModel>
{
public:
    using base_t = txml::XMLNode<Properties<SpecificMappingModel>, SpecificMappingModel>;

    static constexpr std::string_view class_name()
    {
        return "properties";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_OBJECTS_PROPERTIES_HPP
