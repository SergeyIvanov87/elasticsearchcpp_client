#include <fstream>
#include <stdexcept>

#include "bin/utils/cfg_reader.hpp"
#include "bin/utils/parse_string.hpp"

namespace bin
{
namespace utils
{
cfg_reader::cfg_reader(const std::filesystem::path &file_path, const char separator)
{
    std::ifstream file;
    file.open(file_path, std::ifstream::in);
    if(!file.good())
    {
        throw std::runtime_error(std::string("file is not found: ") + file_path.c_str());
    }

    char line[1024];
    while(file.getline(line, 1024))
    {
        auto [name, value] = parse_key_value(line, separator);

        // test on commented/default param
        if (!name.empty() && name[0]=='#')
        {
            const char *default_param_start_ptr = get_next_char_not_if(name.c_str() + 1, ::isspace);
            if (default_param_start_ptr && *default_param_start_ptr != '\0')
            {
                avp_default_map.emplace(std::piecewise_construct,
                                        std::forward_as_tuple(default_param_start_ptr),
                                        std::forward_as_tuple(std::move(value)));
            }
            continue;
        }
        avp_map.emplace(std::move(name), std::move(value));
    }
}

const cfg_reader::value_t &cfg_reader::get(const name_t &name) const
{
    try {
        return get_impl(name, avp_map);
    } catch (...) {}
    return get_impl(name, avp_default_map);
}

cfg_reader::value_t cfg_reader::get_or_default(const name_t &name, value_t &&default_value) const
{
    value_t v = std::move(default_value);
    try {
        v = get_impl(name, avp_map);
    } catch (...) {
        try  {
            v = get_impl(name, avp_default_map);
        } catch (...) {
        }
    }

    return std::move(v);
}

const cfg_reader::value_t &cfg_reader::get_impl(const name_t &name,  const storage_t& s)
{
    auto it = s.find(name);
    if (it == s.end())
    {
        throw std::runtime_error("cfg_reader value not found: " + name);
    }
    return it->second;
}
} // namespace utils
} // namespace bin
