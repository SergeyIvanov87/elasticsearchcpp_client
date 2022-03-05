#ifndef ANSWER_MODEL_SEARCH_OBJECT_IGNORE_UNMAPPED_H
#define ANSWER_MODEL_SEARCH_OBJECT_IGNORE_UNMAPPED_H
#include <string>
#include <txml/txml_fwd.h>

namespace model
{
class UnmappedType: public txml::XMLNodeLeaf<UnmappedType, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<UnmappedType, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "unmapped_type";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    UnmappedType(value_t&& value) :
        base_t(std::move(value))
    {
    }

    UnmappedType(const value_t& value) :
        base_t(value)
    {
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_IGNORE_UNMAPPED_H
