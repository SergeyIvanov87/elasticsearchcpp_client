#ifndef BIN_COMMON_V7_DISPATCHER_HPP
#define BIN_COMMON_V7_DISPATCHER_HPP

#include "bin/common/schema_dispatcher.hpp"

#include "bin/v7/model.hpp"
#include "elasticsearch/service/all_requests.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"

namespace bin
{
using namespace common;
namespace v7
{
class dispatcher
{
    using base_dispatcher_t = bin::common::schema_dispatcher<ALL_BOOKS_REQUEST_LIST,
                                                  ALL_IMAGES_REQUEST_LIST,
                                                  ALL_AUX_REQUEST_LIST,
                                                  elasticsearch::v7::index_mapping_delete::transaction>;
    using peer_t = std::string;
    using cluster_dispatcher_t = std::map<peer_t, std::shared_ptr<base_dispatcher_t>>;
    cluster_dispatcher_t dispatchers;
    dispatcher_settings settings;

    void init_cluster_dispatchers();

    template<class SpecificRequest, class ...SpecificRequestCtorArgs>
    void create_request(const name_t &schema, SpecificRequestCtorArgs &&...args)
    {
        aux_outputstream() << "create \"" << SpecificRequest::name() << "\" on cluster, size: " << settings.hosts.size() << std::endl;
        size_t success = 0;
        for (const auto &h : settings.hosts)
        {
            auto it = dispatchers.find(h);
            if (it == dispatchers.end())
            {
                it = dispatchers.emplace(h,
                                         std::make_shared<base_dispatcher_t>(dispatcher_settings{settings})).first;
            }

            try
            {
                it->second->create_request<SpecificRequest, decltype(h), SpecificRequestCtorArgs...>(
                                                            schema, h,
                                                            std::forward<SpecificRequestCtorArgs>(args)...);
                success ++;
                aux_outputstream() << "created for peer: " << h << std::endl;
            }
            catch (const std::exception &ex)
            {
                aux_outputstream() << "Cannot establish connection request: " << ex.what() << std::endl;
            }
        }
        aux_outputstream() << "successful requests: (" << success << "/" << settings.hosts.size() << ")" << std::endl;
        if (!success)
        {
            aux_outputstream() << "Cluster is not operable" << std::endl;
            throw std::runtime_error(buffered_aux_stream.str());
        }
    }

public:
    dispatcher(dispatcher_settings s, bool with_cluster_logs = true, bool with_request_logs = false);

    std::ostream &data_outputstream() const;
    std::ostream &aux_outputstream() const;
    const dispatcher_settings& get_settings() const;

    void all_index_mapping();
    void book_index_mapping();
    void image_index_mapping();

    void all_index_mapping_delete();
    void book_index_mapping_delete();
    void image_index_mapping_delete();

    void put_data(const char *file_path,
                  const std::map<std::string, std::string>& override_model_params = {},
                  bool ignore_existing = false);

    using csv_data_t = std::tuple<std::string, std::vector<uint8_t>>;
    std::map<std::string, csv_data_t> get_data(const char *index, const char **document_names, size_t documents_count);

    void update_data(const char *file_path, const char *updated_document_id, const std::map<std::string, std::string>& override_model_params = {});

    void rm_data(const char* index, const char *file_path);

    std::map<std::string, std::string> collect_model_data(const char *file_path);

    void search_match(const char* model_name, const std::map<std::string, std::string>& match_params = {}, const std::map<std::string, std::string>& sort_params = {});
    void book_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params = {});
    void image_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params = {});

    void es_info(const char* model_name, std::ostream &out) const;

    template<class SpecificRequest, class ...SpecificRequestArgs>
    request_ptr_t<SpecificRequest> execute_request(const name_t &schema, SpecificRequestArgs &&...args) const
    {
        request_ptr_t<SpecificRequest> ret;
        aux_outputstream() << "execute \"" << SpecificRequest::name() << "\" on cluster, size: " << settings.hosts.size() << std::endl;
        for (const auto &h : settings.hosts)
        {
            auto it = dispatchers.find(h);
            if (it == dispatchers.end() || !it->second)
            {
                throw std::runtime_error("Cannot execute request - dispatcher for peer: \"" + h +
                                         "\" is not found");
            }

            try
            {
                ret = it->second->execute_request<SpecificRequest, SpecificRequestArgs...>(
                                                        schema,
                                                        std::forward<SpecificRequestArgs>(args)...);
                aux_outputstream() << "executed for peer: " << h << std::endl;
            }
            catch (const std::exception &ex)
            {
                aux_outputstream() << "Cannot execute request: " << ex.what() << std::endl;
            }

            if (ret)
            {
                break;
            }
        }

        if (!ret)
        {
            aux_outputstream() << "Cannot execute request: cluster is not operable" << std::endl;
            throw std::runtime_error(buffered_aux_stream.str());
        }
        return ret;
    }

private:
    bool use_request_logging;
    bool use_cluster_logging;
    mutable std::stringstream buffered_aux_stream;

    template<class Tracer>
    void dump_book_search_match_impl(std::ostream& out, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params, Tracer tracer);

    template<class Tracer>
    void dump_request_image_search_match(std::ostream& out, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params, Tracer tracer);
};
} // namespace v7
} // namespace bin

#endif // BIN_COMMON_V7_DISPATCHER_HPP
