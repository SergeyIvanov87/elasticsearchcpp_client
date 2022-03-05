#ifndef TESTS_COMMON_NODES_HPP
#define TESTS_COMMON_NODES_HPP

#include <txml/txml_fwd.h>

namespace tests
{
struct StubLeafNode : public txml::XMLNodeLeaf<StubLeafNode, std::string>
{
    using base_t = txml::XMLNodeLeaf<StubLeafNode, std::string>;

    static constexpr std::string_view class_name()
    {
        return "test_stub_leaf";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    StubLeafNode(std::string value) :
        base_t(std::move(value))
    {
    }
};

struct StubLeafNode_string : public txml::XMLNodeLeaf<StubLeafNode_string, std::string>
{
    using base_t = txml::XMLNodeLeaf<StubLeafNode_string, std::string>;

    static constexpr std::string_view class_name()
    {
        return "test_stub_leaf_string";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    StubLeafNode_string(std::string value) :
        base_t(std::move(value))
    {
    }
};

struct StubLeafNode_int : public txml::XMLNodeLeaf<StubLeafNode_int, int>
{
    using base_t = txml::XMLNodeLeaf<StubLeafNode_int, int>;

    static constexpr std::string_view class_name()
    {
        return "test_stub_leaf_int";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    StubLeafNode_int(int value) :
        base_t(std::move(value))
    {
    }
};

struct StubLeafNode_bool : public txml::XMLNodeLeaf<StubLeafNode_bool, bool>
{
    using base_t = txml::XMLNodeLeaf<StubLeafNode_bool, bool>;

    static constexpr std::string_view class_name()
    {
        return "test_stub_leaf_bool";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    StubLeafNode_bool(bool value) :
        base_t(std::move(value))
    {
    }
};

struct StubModel : public txml::XMLNode<StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string> {
    using base_t = txml::XMLNode<StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>;

    static constexpr std::string_view class_name()
    {
        return "test_stub_model";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // TESTS_COMMON_NODES_HPP
