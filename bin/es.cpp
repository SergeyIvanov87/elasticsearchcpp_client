#include <iostream>

#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/data_model/serializers/schema_serializer.hpp"

#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "utils/cfg_reader.hpp"
#include "bin/cfg_param_names.hpp"
#include "bin/v7/dispatcher.hpp"
#include "bin/utils/parse_string.hpp"

std::map<std::string, std::string> parse_params(const char *multiline_string, char key_value_sep = ':')
{
    std::map<std::string, std::string> ret;
    const char *pStr = multiline_string;//strchr(multiline_string, '\n');
    while(pStr)
    {
        auto [name, value] = bin::utils::parse_key_value(pStr, key_value_sep);
        if (!name.empty() && !value.empty())
        {
            ret.emplace(name, value);
        }
        pStr = strchr(pStr + 1, '\n');
    }
    return ret;
}

bool is_verbose(int argc, char *argv[])
{
    bool verbose = false;
    for (int i = 0; i < argc && !verbose; i++)
    {
        verbose |= !strcmp(argv[i], "--verbose");
    }
    return verbose;
}

bool is_curl_verbose(int argc, char *argv[])
{
    bool verbose = false;
    for (int i = 0; i < argc && !verbose; i++)
    {
        verbose |= !strcmp(argv[i], "--curl_verbose");
    }
    return verbose;
}
int main(int argc, char* argv[])
{
    using namespace bin;
    utils::cfg_reader es_setting(v7::es_yml_default_path, ':');
    auto port = es_setting.get(v7::es_http_port);
    auto cluster_hosts = es_setting.get(v7::discovery_cluster);


    common::dispatcher_settings s;
    s.hosts = utils::cfg_reader::parse_discovery_cluster(cluster_hosts, port);
    s.curl_verbose = is_curl_verbose(argc,argv);

    std::cerr << s.to_string() << std::endl;

    v7::dispatcher d(s, is_verbose(argc, argv));

    std::string binary_name(argv[0]);
    if (binary_name.find("es_pm_init") != std::string::npos)
    {
        d.all_index_mapping();
    }
    else if (binary_name.find("es_pm_deinit") != std::string::npos)
    {
        d.all_index_mapping_delete();
    }
    else if (binary_name.find("es_put") != std::string::npos)
    {
        if (argc < 2)
        {
            std::cerr << "Please set <filename>";
        }
        if (argc == 2)
        {
            d.put_data(argv[1]);
        }
        else
        {
            d.put_data(argv[1], parse_params(argv[2]));
        }
    }
    else if(binary_name.find("es_rm") != std::string::npos)
    {
        if (argc < 3)
        {
            std::cerr << "Please set <index> <type/if>";

        }
        d.rm_data(argv[1], argv[2]);
    }
    else if(binary_name.find("es_search") != std::string::npos)
    {
        if (argc < 2)
        {
            std::cerr << "Please set <model_name>";
        }
        if (argc == 2)
        {
            d.search_match(argv[1]);
        }
        else if (argc == 3)
        {
            d.search_match(argv[1], parse_params(argv[2]));
        }
        else
        {
            d.search_match(argv[1], parse_params(argv[2]), parse_params(argv[3]));
        }
    }
    else if (binary_name.find("es_test") != std::string::npos)
    {
        if (argc < 2)
        {
            std::cerr << "Please set <filename>";
        }
        auto map = d.collect_model_data(argv[1]);
        std::cout << "Document attributes: " << std::endl;
        for (const auto &pair : map)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
}
