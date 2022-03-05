#ifndef ANSWER_MODEL_OBJECT_DASH_PRIMARY_TERM_H
#define ANSWER_MODEL_OBJECT_DASH_PRIMARY_TERM_H

#include <txml/txml_fwd.h>

namespace model
{
class _PrimaryTerm : public txml::XMLNodeLeaf<_PrimaryTerm, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<_PrimaryTerm, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "_primary_term";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _PrimaryTerm(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_PRIMARY_TERM_H
