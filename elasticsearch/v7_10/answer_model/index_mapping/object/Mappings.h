#ifndef ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_OBJECTS_MAPPING_HPP
#define ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_OBJECTS_MAPPING_HPP

#include <txml/txml_fwd.h>

#include "elasticsearch/v7_10/answer_model/index_mapping/object/Properties.h"

namespace model
{
template<class SpecificMappingModel>
class Mappings: public txml::XMLNode<Mappings<SpecificMappingModel>, Properties<SpecificMappingModel>>
{
public:
    using base_t = txml::XMLNode<Mappings<SpecificMappingModel>, Properties<SpecificMappingModel>>;

    static constexpr std::string_view class_name()
    {
        return "mappings";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ELASTICSEARCH_7_10_ANSWER_MODEL_INDEX_MAPPING_OBJECTS_MAPPING_HPP
