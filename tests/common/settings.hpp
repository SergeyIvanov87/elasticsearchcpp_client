#ifndef TESTS_COMMON_SETTING_HPP
#define TESTS_COMMON_SETTING_HPP
#include <string>

namespace tests
{
struct Settings
{
    std::string set_index(std::string new_index)
    {
        std::swap(new_index, index);
        return new_index;
    }

    const std::string get_host() const      { return host; }
    const std::string get_index() const     { return index; }
    bool curl_verbose() const               { return curl_verbose_flag; }
private:
    std::string index           {"stub_index"};
    std::string host            {"http://localhost:9200/"};
    bool curl_verbose_flag      {true};
};
}
#endif // TESTS_COMMON_SETTING_HPP
