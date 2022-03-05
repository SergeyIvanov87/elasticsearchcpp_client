#ifndef BIN_UTILS_PARSE_STRING_HPP
#define BIN_UTILS_PARSE_STRING_HPP

#include <string>
#include <tuple>

namespace bin
{
namespace utils
{
std::tuple<std::string, std::string> parse_key_value(const char *row,
                                                     char separator);


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
}
}
#endif // BIN_UTILS_PARSE_STRING_HPP
