#ifndef ANSWER_MODEL_OBJECT_REASON_H
#define ANSWER_MODEL_OBJECT_REASON_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Reason : public txml::XMLNodeLeaf<Reason, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Reason, std::string>;

    static constexpr std::string_view class_name()
    {
        return "reason";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Reason(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_REASON_H
