#ifndef ANSWER_MODEL_OBJECT_DASH_SOURCE_H
#define ANSWER_MODEL_OBJECT_DASH_SOURCE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
template<class SpecificMappingModel>
class _Source : public txml::XMLNode<_Source<SpecificMappingModel>, SpecificMappingModel>
{
public:
    using base_t = txml::XMLNodeLeaf<_Source<SpecificMappingModel>, SpecificMappingModel>;

    static constexpr std::string_view class_name()
    {
        return "_source";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_SOURCE_H
