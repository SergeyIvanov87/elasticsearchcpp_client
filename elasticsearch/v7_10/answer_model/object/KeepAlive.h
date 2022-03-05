#ifndef ANSWER_MODEL_OBJECT_KEEP_ALIVE_H
#define ANSWER_MODEL_OBJECT_KEEP_ALIVE_H

#include <iostream>
#include <string>

#include <txml/txml_fwd.h>
#include "elasticsearch/utils/conversion.hpp"

namespace model
{
class KeepAlive : public txml::XMLNodeLeaf<KeepAlive, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<KeepAlive, std::string>;

    static constexpr std::string_view class_name()
    {
        return "keep_alive";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    template <class Rep, class Period = std::ratio<1>>
    KeepAlive(std::chrono::duration<Rep, Period> keep_alive) :
        KeepAlive(elasticsearch::utils::duration_to_string(keep_alive)) { }

    KeepAlive(std::string&& str) :
        base_t(std::move(str))
    {
    }
};
}
#endif // ANSWER_MODEL_OBJECT_KEEP_ALIVE_H
