#ifndef COMMON_MODEL_SCHEMA_VERSION_H
#define COMMON_MODEL_SCHEMA_VERSION_H

#include <cstdlib>
#include <vector>
#include <string>
#include <regex>

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace common_model
{
class SchemaVersion: public txml::XMLNodeLeaf<SchemaVersion, std::vector<size_t>>
{
public:
    using base_t = txml::XMLNodeLeaf<SchemaVersion, std::vector<size_t>>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "schema_version";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    SchemaVersion(value_t&& value) :
        base_t(std::move(value))
    {
    }

    SchemaVersion(const value_t& value) :
        base_t(value)
    {
    }

    SchemaVersion(const std::string &flat_list, const std::string &separators) :
        SchemaVersion (value_t{})
    {
        std::regex rcore(separators);
        std::vector<std::string> version_str_vector{std::sregex_token_iterator(flat_list.begin(), flat_list.end(), rcore, -1), std::sregex_token_iterator()};

        value().reserve(version_str_vector.size());
        std::transform(version_str_vector.begin(), version_str_vector.end(), std::back_inserter(value()), [&flat_list] (const std::string &str)
        {
            size_t ret = std::stoull(str.data(), nullptr, 10);
            if (ret == ULONG_MAX or ret == ULLONG_MAX)
            {
                if (errno == ERANGE) {
                    std::stringstream ss;
                    ss << "Invalid version number: " << str << " - in version string representation: " << flat_list << std::endl;
                    throw std::runtime_error(ss.str());
                }
            }
            return ret;
        });
    }
};
}
}
#endif // COMMON_MODEL_SCHEMA_VERSION_H
