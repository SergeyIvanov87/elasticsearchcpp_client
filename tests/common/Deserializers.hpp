#ifndef TESTS_COMMON_DESERIALZIERS_HPP
#define TESTS_COMMON_DESERIALZIERS_HPP

#include <nlohmann/json.hpp>
#include <txml/applications/json/json.hpp>
#include "tests/common/Nodes.hpp"

namespace tests
{
using namespace json;
template<class ParentAggregator>
TXML_PREPARE_DESERIALIZER_DISPATCHABLE_CLASS(StubLeafNodeDeserializer, ParentAggregator, FromJSON,
                                                                       StubLeafNode)
{
    TXML_DESERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;
};
}
#endif // TESTS_COMMON_DESERIALZIERS_HPP
