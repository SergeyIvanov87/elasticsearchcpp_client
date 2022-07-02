#ifndef BIN_UTILS_PARSE_STRING_HPP
#define BIN_UTILS_PARSE_STRING_HPP

#include <string>
#include <tuple>
#include "elasticsearch/utils/strings.hpp"

namespace bin
{
namespace utils
{
std::tuple<std::string, std::string> parse_key_value(const char *row,
                                                     char separator);
}
}
#endif // BIN_UTILS_PARSE_STRING_HPP
