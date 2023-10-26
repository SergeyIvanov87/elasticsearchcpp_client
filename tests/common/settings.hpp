#ifndef TESTS_COMMON_SETTING_HPP
#define TESTS_COMMON_SETTING_HPP
#include <string>
#include <stdlib.h>

namespace tests
{
struct Settings
{
    static const char *get_cfg_env_name() { return "UT_HOST_ADDR"; }
    Settings()
    {
        char* host_addr = getenv(Settings::get_cfg_env_name());
        if (host_addr)
        {
            host = host_addr;
        }
    }
    std::string set_index(std::string new_index)
    {
        std::swap(new_index, es_index);
        return new_index;
    }

    const std::string get_host() const      { return host; }
    const std::string get_index() const     { return es_index; }
    bool curl_verbose() const               { return curl_verbose_flag; }
private:
    std::string es_index           {"stub_index"};
    std::string host            {"http://localhost:9200/"};
    bool curl_verbose_flag      {true};
};
}
#endif // TESTS_COMMON_SETTING_HPP
