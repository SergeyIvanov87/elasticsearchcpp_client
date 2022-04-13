#include "elasticsearch/v7_10/answer_model/search/object/tag_holder.hpp"

#ifndef ANSWER_MODEL_SEARCH_BOOLEAN_TAGS_H
#define ANSWER_MODEL_SEARCH_BOOLEAN_TAGS_H

namespace model
{
namespace search
{
namespace details
{
template<class T, class ...All>
static constexpr bool
enable_for_node_args() { return elasticsearch::utils::is_all_not<T, All...>(); }
}

struct BooleanElementTag {};

struct MustElementTag {};
struct FilterElementTag {};
}
}

#endif // ANSWER_MODEL_SEARCH_BOOLEAN_TAGS_H
