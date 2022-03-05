#ifndef ANSWER_MODEL_OBJECT_DASH_SEQ_NO_H
#define ANSWER_MODEL_OBJECT_DASH_SEQ_NO_H

#include <txml/txml_fwd.h>

namespace model
{
class _SeqNo : public txml::XMLNodeLeaf<_SeqNo, unsigned int>
{
public:
    using base_t = txml::XMLNodeLeaf<_SeqNo, unsigned int>;

    static constexpr std::string_view class_name()
    {
        return "_seq_no";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    _SeqNo(unsigned int code) : base_t(code) {}
};
}
#endif // ANSWER_MODEL_OBJECT_DASH_SEQ_NO_H
