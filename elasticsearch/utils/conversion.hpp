#ifndef ELASTICSEARCH_UTILS_HPP
#define ELASTICSEARCH_UTILS_HPP

#include <chrono>
#include <string>

namespace  elasticsearch
{
namespace utils
{
namespace details
{
inline std::string duration_to_string(std::chrono::nanoseconds val)
{
    return std::to_string(val.count()) + "nanos";
}

inline std::string duration_to_string(std::chrono::microseconds val)
{
    return std::to_string(val.count()) + "micros";
}

inline std::string duration_to_string(std::chrono::milliseconds val)
{
    return std::to_string(val.count()) + "ms";
}

inline std::string duration_to_string(std::chrono::seconds val)
{
    return std::to_string(val.count()) + "s";
}

inline std::string duration_to_string(std::chrono::minutes val)
{
    return std::to_string(val.count()) + "m";
}

inline std::string duration_to_string(std::chrono::hours val)
{
    return std::to_string(val.count()) + "h";
}
/*
inline std::string duration_to_string(std::chrono::days val)
{
    return std::to_string(val.count()) + "d";
}*/
}

template <class Rep, class Period = std::ratio<1>>
inline  std::string duration_to_string(std::chrono::duration<Rep, Period> dur)
{
    return details::duration_to_string(dur);
}
}
}

#endif // ELASTICSEARCH_UTILS_HPP
