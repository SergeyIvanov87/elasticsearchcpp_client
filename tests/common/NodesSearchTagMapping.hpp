#ifndef TESTS_COMMON_NODES_SEARCH_TAG_MAPPING_HPP
#define TESTS_COMMON_NODES_SEARCH_TAG_MAPPING_HPP

#include "tests/common/Nodes.hpp"
#include "elasticsearch/v7_10/request/tags/search/tags.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
namespace must_helper
{
namespace translation
{
template<>
struct table<tests::StubLeafNode_bool> {
    template<class Model>
    using value_t = ::model::search::must::Term<Model, tests::StubLeafNode_bool>;
};

template<>
struct table<tests::StubLeafNode_int> {
    template<class Model>
    using value_t = ::model::search::must::Term<Model, tests::StubLeafNode_int>;
};

template<>
struct table<tests::StubLeafNode_string> {
    template<class Model>
    using value_t = elasticsearch::v7::search::tag::simple_query_string<Model, tests::StubLeafNode_string>;
};
}
}
}
}
}
}

#endif // TESTS_COMMON_NODES_SEARCH_TAG_MAPPING_HPP
