#ifndef ELASTICSEARCH_UTILS_TRAITS_HPP
#define ELASTICSEARCH_UTILS_TRAITS_HPP

#include <type_traits>
#include <utility>

namespace  elasticsearch
{
namespace utils
{
template<class U, class ...All>
constexpr bool is_all() { return std::conjunction_v<std::is_same<std::decay_t<All>, U>...>; }
template<class U, class ...All>
constexpr bool is_all_not() { return !std::conjunction_v<std::is_same<std::decay_t<All>, U>...>; }

template<class U, class ...All>
constexpr bool is_any_not() { return !std::disjunction_v<std::is_same<std::decay_t<All>, U>...>; }

template<class U, class ...All>
constexpr bool is_all_base() { return std::conjunction_v<std::is_base_of<U, All>...>; }
template<class U, class ...All>
constexpr bool is_all_base_not() { return !std::conjunction_v<std::is_base_of<U, All>...>; }

template<class U, class ...All>
constexpr bool is_any_base() { return std::disjunction_v<std::is_base_of<U, All>...>; }

template<class U, class ...All>
constexpr bool is_any_base_not() { return !std::disjunction_v<std::is_base_of<U, All>...>; }

namespace detail
{
    template<std::size_t I, typename Element, typename FirstElement, typename... OtherElements>
    struct tuple_element_index_helper
    {
        static const constexpr bool is_same = std::is_same<Element, FirstElement>::value;
        static const constexpr std::size_t value =
            std::conditional<std::is_same_v<Element, FirstElement>,
                             std::integral_constant<std::size_t, I>,
                             tuple_element_index_helper<I + 1, Element, OtherElements...>>::type::value;
    };

    template<std::size_t I, typename Element, typename FirstElement>
    struct tuple_element_index_helper<I, Element, FirstElement>
    {
        static_assert(std::is_same<Element, FirstElement>::value, "requested Element in list is abset");
        static const constexpr std::size_t value = I;
    };
}

template <class Element, class Tuple>
struct tuple_element_index;

template <class Element, class... TupleElementTypes>
struct tuple_element_index<Element, std::tuple<TupleElementTypes...>> {
    static constexpr std::size_t value = detail::tuple_element_index_helper<0, Element, TupleElementTypes...>::value;
};
template <class Element, class Tuple>
inline constexpr std::size_t tuple_element_index_v = tuple_element_index<Element, Tuple>::value;

}
}
#endif // ELASTICSEARCH_UTILS_TRAITS_HPP
