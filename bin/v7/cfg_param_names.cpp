#include "bin/cfg_param_names.hpp"

namespace bin
{
namespace v7
{
const char *es_yml_default_path = "/etc/elasticsearch/elasticsearch.yml";
const char *es_http_port = "http.port";

const char *es_network_host = "network.host";
const char *discovery_cluster = "discovery.seed_hosts";

const char *schema_indices[] = {"book", "image", nullptr};
const char *doc_id_schema_indices[] = {"book_doc_id", "image_doc_id", nullptr};
} // namespace v7
} // namespace bin
