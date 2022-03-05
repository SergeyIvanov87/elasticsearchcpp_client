#include <algorithm>
#include  <stdexcept>

#include "bin/utils/parse_string.hpp"

namespace bin
{
namespace utils
{
std::tuple<std::string, std::string> parse_key_value(const char *row,
                                                     char separator)
{
    std::tuple<std::string, std::string > pair;
    if(!row)
    {
        throw std::runtime_error("parse_key_value: nothing to proceed");
    }

    const char *pStart, *pEnd;
    pStart = get_next_char_not_if(row, ::isspace);
    pEnd = get_next_char_if(pStart, [separator]( const char* sym) { return (isspace(*sym) || *sym == separator);});
    std::get<0>(pair) = std::string(pStart, pEnd - pStart);

    pStart = get_next_char_if(pEnd, [separator]( const char* sym) { return !(isspace(*sym) || *sym == separator);});
    pEnd = get_next_char_if(pStart, ::isspace);
    std::get<1>(pair) = std::string(pStart, pEnd - pStart);

    return pair;
}
}
}
