#ifndef COMMON_MODEL_BINARY_BLOB_H
#define COMMON_MODEL_BINARY_BLOB_H

#include <vector>
#include <txml/txml_fwd.h>
#include <txml/include/XMLNodeLeaf.hpp>

#include "elasticsearch/utils/base64.h"

namespace elasticsearch
{
namespace common_model
{
class BinaryBlob: public txml::XMLNodeLeaf<BinaryBlob, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<BinaryBlob, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "binary_blob";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    BinaryBlob(value_t&& value) :
        base_t(std::move(value))
    {
    }

    BinaryBlob(std::vector<uint8_t>&& value) :
        base_t(elasticsearch::utils::base64_encode(value))
    {
    }

    std::vector<uint8_t> getEncodedValue() const
    {
        return elasticsearch::utils::base64_decode(base_t::value());
    }
};
}
}
#endif // COMMON_MODEL_BINARY_BLOB_H
