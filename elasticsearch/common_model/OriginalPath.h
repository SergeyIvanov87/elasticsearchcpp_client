#ifndef COMMON_MODEL_ORIGINAL_PATH_H
#define COMMON_MODEL_ORIGINAL_PATH_H

#include <txml/txml_fwd.h>
#include <txml/include/XMLNodeLeaf.hpp>

namespace elasticsearch
{
namespace common_model
{
class OriginalPath: public txml::XMLNodeLeaf<OriginalPath, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<OriginalPath, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "orig_path";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    OriginalPath(value_t&& value) :
        base_t(std::move(value))
    {
    }
};
}
}
#endif // COMMON_MODEL_ORIGINAL_PATH_H
