#ifndef ANSWER_MODEL_SEARCH_OBJECT_TAG_HOLDER_HPP
#define ANSWER_MODEL_SEARCH_OBJECT_TAG_HOLDER_HPP

namespace model
{
namespace search
{
template<class ...Tags>
struct TagHolder : public Tags... {};

template<class Tag, class ...HolderTags>
constexpr bool isContainTag(TagHolder<HolderTags...>)
{
    return std::disjunction_v<std::is_same<Tag, HolderTags>...>;
}

template<class Tag>
constexpr bool isContainTag(...)
{
    return false;
}

template<class Tag, class Target>
constexpr bool has_tag()
{
    return std::is_base_of<Tag, Target>::value;
}

struct QueryElementTag {};
}
}

#endif // ANSWER_MODEL_SEARCH_OBJECT_TAG_HOLDER_HPP
