#ifndef ANSWER_MODEL_SEARCH_OBJECT_RESP_HITSARRAY_H
#define ANSWER_MODEL_SEARCH_OBJECT_RESP_HITSARRAY_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Index.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Id.h"
#include "elasticsearch/v7_10/answer_model/data_creation/object/_Source.h"
#include "elasticsearch/v7_10/answer_model/search/object/resp/_Score.h"
#include "elasticsearch/v7_10/answer_model/object/_Type.h"

namespace model
{
template<class SpecificMappingModel>
class HitsArrayElement : public txml::XMLNode<HitsArrayElement<SpecificMappingModel>,
                                              _Index, _Id, _Score,
                                              _Source<SpecificMappingModel>/*Sort*/, _Type>
{
    public:
    using base_t = txml::XMLNode<HitsArrayElement<SpecificMappingModel>,
                                 _Index, _Id, _Score,
                                 _Source<SpecificMappingModel>/*Sort*/, _Type>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class SpecificMappingModel>
class HitsArray : public txml::XMLArray<HitsArray<SpecificMappingModel>, HitsArrayElement<SpecificMappingModel>>
{
public:
    using element_t = HitsArrayElement<SpecificMappingModel>;
    using base_t = txml::XMLArray<HitsArray<SpecificMappingModel>, element_t>;

    static constexpr std::string_view class_name()
    {
        return "hits";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_RESP_HITSARRAY_H
