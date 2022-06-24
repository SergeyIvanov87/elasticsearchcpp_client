#include "elasticsearch/utils/traits.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/tag_holder.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/tags.hpp"

#include <gtest/gtest.h>

namespace tests
{

struct Base {};
struct A: Base {};
struct B: Base {};
struct Foo {};
struct Bar {};
TEST(Traits, basics)
{
    static_assert(elasticsearch::utils::is_all_not<Base, Foo, Bar>(), "is_all_not failed");
    static_assert(elasticsearch::utils::is_all_base_not<Base, Foo, Bar>(), "is_all_base_not failed");
    static_assert(elasticsearch::utils::is_all_base<Base, A, B>(), "is_all_base failed");
    static_assert(!elasticsearch::utils::is_all_base_not<Base, A, B>(), "not is_all_base_not failed");
}

struct TagA {};
struct TagB {};
struct TagC {};

struct TaggedA : model::search::TagHolder<TagA> {};
struct TaggedB : model::search::TagHolder<TagB> {};
struct TaggedAB : model::search::TagHolder<TagA, TagB> {};
struct TaggedABC : model::search::TagHolder<TagA, TagB, TagC> {};
struct UnTagged {};

TEST(Tags, basics)
{
    static_assert(model::search::has_tag<TagA, TaggedA>(), "TaggedA failed for TagA");
    static_assert(!model::search::has_tag<TagB, TaggedA>(), "TaggedA sucess for TagB");

    static_assert(model::search::has_tag<TagB, TaggedB>(), "TaggedB failed for TagB");
    static_assert(!model::search::has_tag<TagA, TaggedB>(), "TaggedA sucess for TagA");

    static_assert(model::search::has_tag<TagA, TaggedAB>(), "TaggedAB failed for TagA");
    static_assert(model::search::has_tag<TagB, TaggedAB>(), "TaggedAB failed for TagB");
    static_assert(!model::search::has_tag<TagC, TaggedAB>(), "TaggedAB sucess for TagC");

    static_assert(model::search::has_tag<TagC, TaggedABC>(), "TaggedABC failed for TagC");

    static_assert(!model::search::has_tag<TagA, UnTagged>(), "UnTagged sucess for TagA");
    static_assert(!model::search::has_tag<TagB, UnTagged>(), "UnTagged sucess for TagB");
    static_assert(!model::search::has_tag<TagC, UnTagged>(), "UnTagged sucess for TagC");
}

namespace details
{
template <class Target>
struct is_must_element : std::integral_constant<bool, model::search::has_tag<model::search::MustElementTag, Target>()> {};
template<class ...All>
static constexpr bool
enable_for_must_element() {return  std::conjunction_v<is_must_element<All>()...>; }

template <class Target>
struct is_filter_element : std::integral_constant<bool, model::search::has_tag<model::search::FilterElementTag, Target>()> {};

template<class ...All>
static constexpr bool
enable_for_filter_element() {return  std::conjunction_v<is_filter_element<All>()...>; }
}

struct MustElementA : model::search::TagHolder<model::search::MustElementTag> {};
struct FilterElementA : model::search::TagHolder<model::search::FilterElementTag> {};
TEST(MustTag, basics)
{
    static_assert(details::is_must_element<MustElementA>(), "MustElementA failed for MustElementTag");
    static_assert(!details::is_filter_element<MustElementA>(), "MustElementA success for FilterElementTag");

    static_assert(!details::is_must_element<FilterElementA>(), "FilterElementA success for MustElementTag");
    static_assert(details::is_filter_element<FilterElementA>(), "FilterElementA failed for FilterElementTag");
}
}
