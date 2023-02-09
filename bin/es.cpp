#include <filesystem>
#include <iostream>
#include <iterator>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "elasticsearch/utils/base64.h"
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

std::map<std::string, std::string> parse_params_list(const char **multiline_string, size_t multiline_amount, char key_value_sep = param_key_value_separator)
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

bool is_verbose(int argc, const char *argv[])
{
    bool verbose = false;
    for (int i = 0; i < argc && !verbose; i++)
    {
        verbose |= !strcmp(argv[i], "--verbose");
    }
    return verbose;
}

bool find_param_by_name(const char *param_name, int argc, const char *argv[])
{
    bool name_exist = false;
    auto param_name_size = strlen(param_name);
    for (int i = 0; i < argc && !name_exist; i++)
    {
        name_exist |= !strncmp(argv[i], param_name, param_name_size);
    }
    return name_exist;
}

bool is_curl_verbose(int argc, const char *argv[])
{
    return find_param_by_name("--curl_verbose", argc, argv);
}

bool is_cluster_verbose(int argc, const char *argv[])
{
    return !find_param_by_name("--no_cluster_verbose", argc, argv);
}

bool is_force(int argc, const char *argv[])
{
    return find_param_by_name("--force", argc, argv);
}

namespace documents
{
    struct tmp_entry
    {
        tmp_entry(std::string_view doc_name, const bin::v7::dispatcher::csv_data_t &in);
        tmp_entry(tmp_entry &&);
        tmp_entry& operator=(tmp_entry &&);
        ~tmp_entry();

        tmp_entry(const tmp_entry &) = delete;
        tmp_entry& operator=(const tmp_entry &) = delete;

        std::filesystem::path get_path() const;
        const std::string& get_requested_doc_name() const;

        int release_fd() &&;
    private:
        int fd;
        std::string requested_doc_name;
        std::filesystem::path full_path;
    };

    struct permanent_entry
    {
        std::filesystem::path get_full_name() const { return destination_full_path; }

        std::filesystem::path destination_full_path;
    };

    struct permanent_original_path_entry : public permanent_entry
    {
        permanent_original_path_entry(tmp_entry &&in, std::filesystem::path prefix_path);
    private:
        std::string src_local_name;

    };

    struct permanent_plain_entry : public permanent_entry
    {
        permanent_plain_entry(tmp_entry &&in, std::filesystem::path prefix_path);
    private:
        std::string src_local_name;
    };

    template<class T, class ...U>
    std::vector<T> tmp_entry_extractor(std::vector<tmp_entry> &tmp_entries,
                                       std::filesystem::path extract_prefix,
                                       int &error_number,
                                       U &&...args);
}


int main(int argc, const char* argv[])
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
    else if (binary_name.find("es_csv_get") != std::string::npos)
    {
        const char *csv_header[] = {"document_name", "restored_file_path", "base64_file_data", nullptr};
        if (argc < 3)
        {
            std::cerr << "Please set <index> <document_name_1> ... <document_name_N>" << std::endl;
            std::cerr << "\nA command result must be in a CSV format.\n";
            std::cerr << "It's urged using linux command pipelining to extract document data in file:\n";
            std::cerr << "\nExample:\n\n";
            std::cerr << "./es_csv_get book _doc/20 | awk -F, '{print $3}' | base64 -di > \"_doc_20.epub\"\n";
            std::cerr << std::endl;
            std::cerr << "Multiple files can be retrieved by command:\n\n";
            std::cerr << R"(/es_csv_get book _doc/20 _doc/19 | awk -F, '/_doc\/20/{print $3 > "_doc_20.epub"}/_doc\/19/{print $3 > "_doc_19.epub"}')" << std::endl;
            std::cerr << "CVS_HEADER:\n";
            const char **index = csv_header;
            do
            {
                std::cerr << *index << ",";
            } while(*(++index));
            std::cerr << std::endl;
            return -1;
        }
        std::map<std::string, bin::v7::dispatcher::csv_data_t> ret_data;
        int requested_doc_num = argc - 2;
        try
        {
            ret_data = d.get_data(argv[1], argv + 2, requested_doc_num);
        }
        catch (const std::exception &ex)
        {
            std::cerr << ex.what() << std::endl;
            return -1;
        }

        //to form csv
        for(const auto ret_data_it : ret_data)
        {
            std::cout << ret_data_it.first << ",";
            std::cout << std::get<0>(ret_data_it.second) << ",";
            std::cout << elasticsearch::utils::base64_encode(std::get<1>(ret_data_it.second)) << ",";
            std::cout << std::endl;
        }
        return 0;
    }
    else if (binary_name.find("es_get") != std::string::npos)
    {
        const char *keys[] = {"--extract-path", "--use-original-path", "--group-size"};
        std::filesystem::path extract_path_value("./");
        bool use_original_path_value = 0;
        if (argc < 3)
        {
            std::cerr << "Please set <index> <document_name_1> ... <document_name_N>" << std::endl;
            std::cerr << "\nParams:\n";
            std::cerr << *keys[0] << "\t\t\t[default = " << extract_path_value << "] Override directory mount point from where the files are extracted\n";
            std::cerr << *keys[1] << "\t\t\t[default = " << use_original_path_value << "] Recreate the files in original directories hierarchy instead of plain document names\n";
            std::cerr << *keys[2] << "\t\t\t[default = unlimited] Use several request series by group size files amount to prevent extra memory consumption. Increase processing time\n";
            std::cerr << std::endl;
        }

        // determine documents count from input params
        // all params which are not configuration keys treat as documents to be requested
        const int doc_arg_index = 2;
        int i = doc_arg_index;
        while(i < argc) {
            bool arg_found = false;
            for (size_t j = 0; j < sizeof(keys) / sizeof(keys[0]); j ++)
            {
                if(strstr(argv[i], keys[j]))
                {
                    arg_found = true;
                    break; // find first key in argv
                }
            }
            if (arg_found) break;
            i++;
        }

        // check documents amount
        int requested_doc_num = i - doc_arg_index;
        if (requested_doc_num == 0)
        {
            std::cerr << "No documents requested, no job to do" << std::endl;
            return 0;
        }

        // parse params
        std::map<std::string, std::string> key_value_pairs = parse_params_list(argv + i, argc - i, '=');
        if (auto it = key_value_pairs.find(keys[0]); it != key_value_pairs.end())
        {
            extract_path_value = it->second;
        }

        if (auto it = key_value_pairs.find(keys[1]); it != key_value_pairs.end())
        {
            std::stringstream ss;
            ss << it->second;
            ss >> use_original_path_value;
        }

        int group_size = requested_doc_num;
        if (auto it = key_value_pairs.find(keys[2]); it != key_value_pairs.end())
        {
            std::stringstream ss;
            ss << it->second;
            int group_size_candidate = 0;
            ss >> group_size_candidate;
            group_size = std::min(group_size, group_size_candidate);
        }

        std::map<std::string, bin::v7::dispatcher::csv_data_t> ret_data;
        std::vector<documents::tmp_entry> restored_documents;

        // request for documents
        int skipped_err_files = 0;
        for (int i = 0; i < requested_doc_num; i += group_size)
        {
            size_t requested_document_count = std::min(requested_doc_num - i, group_size);
            try
            {
                ret_data = d.get_data(argv[1], argv + doc_arg_index + i, requested_document_count);
            }
            catch (const std::exception &ex)
            {
                std::cerr << "Cannot get some or more documents:\n" << std::endl;
                for (int j = 0; j < requested_document_count; j ++)
                {
                    std::cerr << *(argv + doc_arg_index + i + j) << std::endl;
                }
                std::cerr << "Skip them. Error: " << ex.what() << std::endl;

                skipped_err_files += requested_document_count;
                continue;
            }

            // create temporary files
            size_t prev_records = restored_documents.size();
            restored_documents.reserve(prev_records + ret_data.size());
            for(const auto ret_data_it : ret_data)
            {
                try
                {
                    restored_documents.emplace_back(ret_data_it.first, ret_data_it.second);
                }
                catch(const std::exception& ex)
                {
                    std::cerr << ex.what() << std::endl;
                    skipped_err_files++;
                }
            }
        }

        // create path for renaming temporary files
        std::filesystem::path extract_path(extract_path_value);
        std::vector<std::filesystem::path> finalized_file_paths;
        if (use_original_path_value)
        {
            for (auto it = restored_documents.begin(); it != restored_documents.end(); )
            {
                std::filesystem::path dir(extract_path);
                dir += it->get_path().remove_filename();
                try
                {
                    std::filesystem::create_directories(dir);
                    ++it;
                }
                catch(const std::exception &ex)
                {
                    std::cerr << "Cannot create directory: " << dir
                              << ". Error: " << ex.what() << std::endl;
                    it = restored_documents.erase(it);
                    skipped_err_files++;
                }
            }
            //rename files
            auto files = documents::tmp_entry_extractor<documents::permanent_original_path_entry>(restored_documents,
                                                                                                  extract_path,
                                                                                                  skipped_err_files);
            std::transform(files.begin(), files.end(), std::back_inserter(finalized_file_paths),
                           [] (const auto &f) {return f.get_full_name();});
        }
        else
        {
            //rename files
            auto files = documents::tmp_entry_extractor<documents::permanent_plain_entry>(restored_documents,
                                                                                          extract_path,
                                                                                          skipped_err_files);
            std::transform(files.begin(), files.end(), std::back_inserter(finalized_file_paths),
                           [] (const auto &f) {return f.get_full_name();});
        }

        // print renames files
        size_t j = 0;
        for (const auto &p : finalized_file_paths)
        {
            std::cout << j++ << ":" << p << std::endl;
        }
        // print footer
        if (skipped_err_files != 0)
        {
            std::cerr << "Total number of documents have been skipped: " << skipped_err_files << std::endl;
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



namespace documents
{
tmp_entry::tmp_entry(std::string_view doc_name, const bin::v7::dispatcher::csv_data_t &in) :
    fd(-1),
    requested_doc_name(doc_name)
{
    full_path = std::get<0>(in);
    fd = open("./", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR | O_NOATIME | O_CLOEXEC);
    if (fd == -1)
    {
        std::stringstream ss;
        ss << "Cannot create temporary file for document: " << doc_name
           << ". Skip it. Error: " << strerror(errno);

        throw std::runtime_error(ss.str());
    }

    const std::vector<uint8_t> &doc_data = std::get<1>(in);
    size_t size_to_write = doc_data.size();
    size_t written_size = 0;
    do
    {
        int ret = write(fd, doc_data.data(), size_to_write - written_size);
        if (ret == -1)
        {
            if (errno != EINTR)
            {
                std::stringstream ss;
                ss << "An error occured when process was writing to file from document: " << doc_name
                   << ". Skip it. Error: " << strerror(errno);

                close(fd);
                throw std::runtime_error(ss.str());
            }
            continue;
        }
        written_size += ret;
    } while(written_size != size_to_write);
}
tmp_entry::tmp_entry(tmp_entry &&src) :
    fd(src.fd),
    full_path(std::move(src.full_path))
{
    src.fd = -1;
}

tmp_entry& tmp_entry::operator=(tmp_entry &&src)
{
    if (this == &src)
    {
        return *this;
    }

    full_path = std::move(src.full_path);
    fd = std::move(src).release_fd();
    return *this;
}

tmp_entry::~tmp_entry()
{
    if (fd != -1)
    {
        close(fd);
    }
}

std::filesystem::path tmp_entry::get_path() const
{
    return full_path;
}

const std::string& tmp_entry::get_requested_doc_name() const
{
    return requested_doc_name;
}

int tmp_entry::release_fd() &&
{
    int ret_fd = fd;
    fd = -1;
    return ret_fd;
}

permanent_original_path_entry::permanent_original_path_entry(tmp_entry &&in, std::filesystem::path prefix_path)
{
    destination_full_path = prefix_path;
    if (in.get_path().is_absolute())
    {
        destination_full_path += in.get_path();
    }
    else
    {
        destination_full_path /= in.get_path();
    }
    int fd = std::move(in).release_fd();
    char path[PATH_MAX];
    snprintf(path, PATH_MAX,  "/proc/self/fd/%d", fd);
    int res = linkat(AT_FDCWD, path, AT_FDCWD, destination_full_path.string().c_str(), AT_SYMLINK_FOLLOW);
    if (res == -1)
    {
        std::stringstream ss;
        ss << "Cannot rename temporary file to: " << destination_full_path
           << ". Error: " << strerror(errno);

        close(fd);
        throw std::runtime_error(ss.str());
    }

    // release initial fd
    close(fd);
}

permanent_plain_entry::permanent_plain_entry(tmp_entry &&in, std::filesystem::path prefix_path)
{
    std::string requested_doc_name = in.get_requested_doc_name();
    std::replace(requested_doc_name.begin(), requested_doc_name.end(), '/', '_');
    std::filesystem::path requested_doc_name_path(requested_doc_name);
    destination_full_path = prefix_path;
    if (requested_doc_name_path.is_absolute())
    {
        destination_full_path += requested_doc_name_path;
    }
    else
    {
        destination_full_path /= requested_doc_name_path;
    }

    int fd = std::move(in).release_fd();

    char path[PATH_MAX];
    snprintf(path, PATH_MAX,  "/proc/self/fd/%d", fd);
    int res = linkat(AT_FDCWD, path, AT_FDCWD, destination_full_path.string().c_str(), AT_SYMLINK_FOLLOW);
    if (res == -1)
    {
        std::stringstream ss;
        ss << "Cannot rename temporary file to: " << destination_full_path
           << ". Error: " << strerror(errno);

        close(fd);
        throw std::runtime_error(ss.str());
    }

    // release initial fd
    close(fd);
}

template <class T, class ... U>
std::vector<T> tmp_entry_extractor(std::vector<tmp_entry> &tmp_entries,
                                   std::filesystem::path extract_prefix,
                                   int &error_number,
                                   U &&...args)
{
    std::vector<T> copied_files;
    copied_files.reserve(tmp_entries.size());
    for(auto &&v : tmp_entries)
    {
        try
        {
            copied_files.emplace_back(std::move(v), extract_prefix, std::forward<U>(args)...);
        }
        catch(const std::exception &ex)
        {
            std::cerr << ex.what() << std::endl;
            error_number++;
        }
    }
    return copied_files;
}
}
