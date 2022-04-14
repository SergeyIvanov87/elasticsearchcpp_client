#ifndef ANSWER_MODEL_SEARCH_OBJECT_TAG_HOLDER_HPP
#define ANSWER_MODEL_SEARCH_OBJECT_TAG_HOLDER_HPP

namespace model
{
namespace search
{
template<class ...Tags>
struct TagHolder : public Tags... {};


template<class T, class ...All>
static constexpr bool
all_of_node_child() { return elasticsearch::utils::is_all_not<T, All...>(); }

template<class Tag, class Target>
constexpr bool has_tag()
{
    return std::is_base_of<Tag, Target>::value;
}

template <class Tag, class Target>
struct has_tag_v : std::integral_constant<bool, model::search::has_tag<Tag, Target>()> {};

template<class Tag, class ...All>
static constexpr bool
all_of_tag() {return  std::conjunction_v<has_tag_v<Tag, std::decay_t<All>>...>; }



struct QueryElementTag {};
}
}

#endif // ANSWER_MODEL_SEARCH_OBJECT_TAG_HOLDER_HPP
