#ifndef ANSWER_MODEL_OBJECT_ROOT_CAUSE_H
#define ANSWER_MODEL_OBJECT_ROOT_CAUSE_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/object/Type.h"
#include "elasticsearch/v7_10/answer_model/object/Reason.h"

namespace model
{
template<class ...Ext>
class RootCauseElement : public txml::XMLNode<RootCauseElement<Ext...>, Type, Reason, Ext...> // Index, IndexUUID> {
{
    public:
    using base_t = txml::XMLNode<RootCauseElement<Ext...>, Type, Reason, Ext...>;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class ...Ext>
class RootCause : public txml::XMLArray<RootCause<Ext...>, RootCauseElement<Ext...>>
{
public:
    using element_t = RootCauseElement<Ext...>;
    using base_t = txml::XMLArray<RootCause<Ext...>, element_t>;

    static constexpr std::string_view class_name()
    {
        return "root_cause";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_OBJECT_ROOT_CAUSE_H
