#ifndef BIN_UTILS_CFG_READER_HPP
#define BIN_UTILS_CFG_READER_HPP

#include <filesystem>
#include <list>
#include <map>
#include <string>

namespace bin
{
namespace utils
{
class cfg_reader
{
public:
    using name_t = std::string;
    using value_t = std::string;

    cfg_reader(const std::filesystem::path &file_path, const char separator);

    const value_t &get(const name_t &name) const;
    value_t get_or_default(const name_t &name, value_t &&default_value) const;

    static std::list<std::string> parse_discovery_cluster(const std::string &cluster_list, const std::string &default_port = "9200", const std::string &url_prefix = "http://");
private:
    using storage_t = std::map<name_t, value_t>;

    static const value_t &get_impl(const name_t &name, const storage_t& s);
    storage_t avp_map;
    storage_t avp_default_map;
};
} // namespace utils
} // namespace bin
#endif // BIN_UTILS_CFG_READER_HPP
