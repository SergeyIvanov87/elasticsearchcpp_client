#ifndef ELASTICSEARCH_UTILS_TRAITS_HPP
#define ELASTICSEARCH_UTILS_TRAITS_HPP

#include <type_traits>
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
}
}
#endif // ELASTICSEARCH_UTILS_TRAITS_HPP
