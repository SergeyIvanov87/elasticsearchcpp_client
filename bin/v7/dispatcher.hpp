#ifndef BIN_COMMON_V7_DISPATCHER_HPP
#define BIN_COMMON_V7_DISPATCHER_HPP

#include "bin/common/schema_dispatcher.hpp"

#include "bin/v7/model.hpp"
#include "elasticsearch/aux/all_requests.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"

namespace bin
{
using namespace common;
namespace v7
{
class dispatcher: public bin::common::schema_dispatcher<ALL_BOOKS_REQUEST_LIST,
                                                        ALL_IMAGES_REQUEST_LIST,
                                                        ALL_AUX_REQUEST_LIST,
                                                        elasticsearch::v7::index_mapping_delete::transaction>
{
    using base_t = bin::common::schema_dispatcher<ALL_BOOKS_REQUEST_LIST,
                                                  ALL_IMAGES_REQUEST_LIST,
                                                  ALL_AUX_REQUEST_LIST,
                                                  elasticsearch::v7::index_mapping_delete::transaction>;
public:
    dispatcher(dispatcher_settings s, bool with_logs = false);

    void all_index_mapping();
    void book_index_mapping();
    void image_index_mapping();

    void all_index_mapping_delete();
    void book_index_mapping_delete();
    void image_index_mapping_delete();

    void put_data(const char *file_path,
                  const std::map<std::string, std::string>& override_model_params = {},
                  bool ignore_existing = false);

    void update_data(const char *file_path, const char *updated_document_id, const std::map<std::string, std::string>& override_model_params = {});

    void rm_data(const char* index, const char *file_path);

    std::map<std::string, std::string> collect_model_data(const char *file_path);

    void search_match(const char* model_name, const std::map<std::string, std::string>& match_params = {}, const std::map<std::string, std::string>& sort_params = {});
    void book_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params = {});
    void image_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params = {});
private:
    bool use_logging;

    template<class Tracer>
    void dump_book_search_match_impl(std::ostream& out, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params, Tracer tracer);

    template<class Tracer>
    void dump_request_image_search_match(std::ostream& out, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params, Tracer tracer);
};
} // namespace v7
} // namespace bin

#endif // BIN_COMMON_V7_DISPATCHER_HPP
