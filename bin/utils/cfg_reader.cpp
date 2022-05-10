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
            const char *default_param_start_ptr = elasticsearch::utils::get_next_char_not_if(name.c_str() + 1, ::isspace);
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

std::list<std::string> cfg_reader::parse_discovery_cluster(const std::string &cluster_list, const std::string &default_port, const std::string &url_prefix)
{
    std::list<std::string> ret;
    const char *peer_it = elasticsearch::utils::get_next_char_not_if(cluster_list.c_str(), [] (const char *s) {return *s =='[' || *s=='"';});
    while(peer_it && *peer_it)
    {
        const char *peer_next_it = elasticsearch::utils::get_next_char_if(peer_it + 1, [] (const char *s) {return *s =='"';});
        if (peer_next_it && *peer_next_it)
        {
            if (!default_port.empty())
            {
                ret.push_back(url_prefix + std::string(peer_it, peer_next_it) + ":" + default_port);
            }
            else
            {
                ret.push_back(url_prefix + std::string(peer_it, peer_next_it));
            }
        }
        peer_it = elasticsearch::utils::get_next_char_if(peer_next_it + 1, [] (const char *s) {return *s =='"';});
    }
    return ret;
}
} // namespace utils
} // namespace bin
