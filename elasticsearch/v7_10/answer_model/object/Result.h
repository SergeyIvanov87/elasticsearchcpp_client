#ifndef ANSWER_MODEL_OBJECT_RESULT_H
#define ANSWER_MODEL_OBJECT_RESULT_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>

namespace model
{
class Result : public txml::XMLNodeLeaf<Result, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Result, std::string>;

    static constexpr std::string_view class_name()
    {
        return "result";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Result(std::string&& str) : base_t(std::move(str)) {}
};
}
#endif // ANSWER_MODEL_OBJECT_RESULT_H
