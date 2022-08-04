#include <iostream>
#include <string.h>

#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/data_model/serializers/schema_serializer.hpp"

#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "utils/cfg_reader.hpp"
#include "bin/cfg_param_names.hpp"
#include "bin/v7/dispatcher.hpp"
#include "bin/utils/parse_string.hpp"

char param_key_value_separator = ':';
char param_list_item_separator = '\n';

std::map<std::string, std::string> parse_params(const char *multiline_string, char key_value_sep = param_key_value_separator)
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
        pStr = strchr(pStr + 1, param_list_item_separator);
    }
    return ret;
}

std::map<std::string, std::string> parse_params_list(char **multiline_string, size_t multiline_amount, char key_value_sep = param_key_value_separator)
{
    if(multiline_amount == 1)
    {
        return parse_params(*multiline_string, key_value_sep);
    }
    std::map<std::string, std::string> ret;
    size_t index = 0;
    while(index < multiline_amount)
    {
        const char *pStr = multiline_string[index];
        auto [name, value] = bin::utils::parse_key_value(pStr, key_value_sep);
        if (!name.empty() && !value.empty())
        {
            ret.emplace(name, value);
        }
        ++index;
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

bool is_param_exist(int argc, char *argv[], const char *param_name)
{
    bool name_exist = false;
    auto param_name_size = strlen(param_name);
    for (int i = 0; i < argc && !name_exist; i++)
    {
        name_exist |= !strncmp(argv[i], param_name, param_name_size);
    }
    return name_exist;
}

bool is_curl_verbose(int argc, char *argv[])
{
    return is_param_exist(argc, argv, "--curl_verbose");
}

bool is_cluster_verbose(int argc, char *argv[])
{
    return !is_param_exist(argc, argv, "--no_cluster_verbose");
}

bool is_force(int argc, char *argv[])
{
    return is_param_exist(argc, argv, "--force");
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

    bool cluster_logs = is_cluster_verbose(argc, argv);
    if (cluster_logs)
    {
        std::cerr << s.to_string() << std::endl;
    }

    v7::dispatcher d(s, cluster_logs, is_verbose(argc, argv));

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
            std::cerr << "Please set <filename>" << std::endl;
            return -1;
        }
        try
        {
            if (argc == 2)
            {
                d.put_data(argv[1]);
            }
            else
            {
                d.put_data(argv[1], parse_params_list(argv + 2, argc - 2),
                           is_force(argc, argv));
            }
        }
        catch (const std::exception &ex)
        {
            std::cerr << ex.what() << std::endl;
            return -1;
        }
    }
    else if(binary_name.find("es_rm") != std::string::npos)
    {
        if (argc < 3)
        {
            std::cerr << "Please set <index> <type/if>" << std::endl;
            return -1;
        }
        d.rm_data(argv[1], argv[2]);
    }
    else if (binary_name.find("es_info") != std::string::npos)
    {
        using namespace bin::v7;
        if (argc == 1)
        {
            std::cout << "Available schemas:" << std::endl;
            std::string list("\t");
            for (const char **s = schema_indices; *s; s++)
            {
                list =  list + *s + ',';
            }
            if (!list.empty()) list.pop_back();
            std::cout << list<< std ::endl;
            return 0;
        }
        if (argc >= 2)
        {
            const char *found_schema = nullptr;
            size_t argv_len = strlen(argv[1]);
            for (const char **s = schema_indices; *s; s++)
            {
                size_t s_len = strlen(*s);
                if (argv_len == s_len && !strncmp(argv[1], *s, s_len))
                {
                    found_schema = *s;
                    break;
                }
            }
            if (!found_schema)
            {
                std::cout << "Unexpected schema: " << argv[1] << ". Check on list of available schemas:" <<std::endl;
                for (const char **s = schema_indices; *s; s++)
                {
                    std::cout << *s << std ::endl;
                }
                return -1;
            }
            d.es_info(found_schema, std::cout);
            std::cout << "\n!!! Separators for:\n"
                      << "\tparams in list: " << param_key_value_separator << "\n"
                      << "\tparams key/value: \\n" << std::endl
                      << "Example:\n"
                      << "\"param_key_1:param_value_1" << param_list_item_separator
                      << "\"param_key_2:param_value_2" << param_list_item_separator << "\"" << std::endl;
            return 0;
        }
    }
    else if(binary_name.find("es_search") != std::string::npos)
    {
        if (argc < 2)
        {
            std::cerr << "Please set <model_name>" << std::endl;
            return -1;
        }
        if (argc == 2)
        {
            d.search_match(argv[1]);
        }
        else if (argc == 3)
        {
            d.search_match(argv[1], parse_params_list(argv + 2, argc - 2));
        }
        else
        {
            //d.search_match(argv[1], parse_params(argv[2]), parse_params(argv[3])); TODO no sort at now
            d.search_match(argv[1], parse_params_list(argv + 2, argc - 2));
        }
    }
    else if (binary_name.find("es_test") != std::string::npos)
    {
        if (argc < 2)
        {
            std::cerr << "Please set <filename>" << std::endl;
            return -1;
        }
        auto map = d.collect_model_data(argv[1]);
        for (const auto &pair : map)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
    return 0;
}
