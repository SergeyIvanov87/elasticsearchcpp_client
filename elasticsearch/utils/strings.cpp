#include "elasticsearch/utils/strings.hpp"

namespace elasticsearch
{
namespace utils
{
range_descriptor::range_descriptor(value_t && v, bool enclosed) :
    value(std::move(v)),
    border_included(enclosed)
{
}

std::tuple<std::optional<range_descriptor>, std::optional<range_descriptor>>
get_range_values(const std::string &range_contained_string, char sep, std::initializer_list<std::string> filter_out)
{
    std::optional<range_descriptor> gt;
    std::optional<range_descriptor> lt;
    auto val_it = range_contained_string.begin();
    for (auto it = range_contained_string.begin(); it != range_contained_string.end(); )
    {
        if (*it == '[')
        {
            gt = std::make_optional<range_descriptor>("", true);
            ++it;
            val_it = it;
            continue;
        }
        else if (*it == '(')
        {
            gt = std::make_optional<range_descriptor>("", false);
            ++it;
            val_it = it;
            continue;
        }
        else if (*it == ']')
        {
            if (!gt.has_value()) { throw std::runtime_error("Invalid range format: beginning value is missed. Expected: (x,y) or [x,y], got: " + range_contained_string); }
            lt = std::make_optional<range_descriptor>(std::string(val_it, it), true);
            ++it;
            continue;
        }
        else if (*it == ')')
        {
            if (!gt.has_value()) { throw std::runtime_error("Invalid range format: beginning value is missed. Expected: (x,y) or [x,y], got: " + range_contained_string); }
            lt = std::make_optional<range_descriptor>(std::string(val_it, it), false);
            ++it;
            continue;
        }
        else if (*it == sep)
        {
            if (!gt.has_value()) { throw std::runtime_error("Invalid range format. Expected: (x,y) or [x,y], got: " + range_contained_string); }
            gt.value().value = std::string(val_it, it);
            ++it;
            val_it = it;
            continue;
        }
        ++it;
    }

    if (!gt.has_value() or !lt.has_value()) { throw std::runtime_error("Invalid range format: something missed. Expected: (x,y) or [x,y], got: " + range_contained_string); }
    for (const auto &e : filter_out)
    {
        if(gt.has_value() && gt.value().value == e) { gt.reset(); }
        if(lt.has_value() && lt.value().value == e) { lt.reset(); }
    }
    return std::make_tuple(gt, lt);
}
} // namespace utils
} // namespace elasticsearch
