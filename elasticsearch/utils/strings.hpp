#ifndef ES_UTILS_STRINGS_H
#define ES_UTILS_STRINGS_H

#include <array>
#include <iostream>
#include <optional>
#include <string_view>
#include <tuple>


template <std::string_view const&... Strs>
struct join
{
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Strs.size() + ... + 0);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const& s) mutable {
            for (auto c : s) arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // View as a std::string_view
    static constexpr std::string_view value {arr.data(), arr.size() - 1};
};
// Helper to get the value out
template <const std::string_view &... Strs>
static constexpr auto join_v = join<Strs...>::value;

namespace elasticsearch
{
namespace utils
{
template <class Pred>
inline const char *get_next_char_not_if(const char *data, Pred pred)
{
    if(!data) return nullptr;

    while(*data && pred(data))
    {
        ++data;
    }
    return data;
}
//specialization for clib function
template <>
inline const char *get_next_char_not_if(const char *data, int (*clib_pred)(int) noexcept)
{
    if(!data) return nullptr;

    while(*data && (*clib_pred)(*data))
    {
        ++data;
    }
    return data;
}

template <class Pred>
inline const char *get_next_char_if(const char *data, Pred pred)
{
    if(!data) return nullptr;

    while(*data && !pred(data))
    {
        ++data;
    }
    return data;
}
//specialization for clib function
template <>
inline const char *get_next_char_if(const char *data, int (*clib_pred)(int) noexcept)
{
    if(!data) return nullptr;

    while(*data && !(*clib_pred)(*data))
    {
        ++data;
    }
    return data;
}

struct range_descriptor {
    using value_t = std::string;

    range_descriptor(value_t && v, bool enclosed);

    bool border_included = false;
    value_t value;
};

std::tuple<std::optional<range_descriptor>, std::optional<range_descriptor>>
    get_range_values(const std::string &range_contained_string, char sep = ',', std::initializer_list<std::string> filter_out = {"nan", "null", ""});
} // namespace utils
} // namespace elasticsearch
#endif // ES_UTILS_STRINGS_H
