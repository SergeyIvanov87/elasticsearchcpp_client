#ifndef ELASTICSEARCH_AUX_ALL_REQUESTS_HPP
#define ELASTICSEARCH_AUX_ALL_REQUESTS_HPP

#include "elasticsearch/service/doc_id_create.hpp"
#include "elasticsearch/service/doc_id_pm.hpp"

#define ALL_AUX_REQUEST_LIST                                                                       \
                            elasticsearch::aux::index_mapping::transaction,                        \
                            elasticsearch::aux::create_doc_id::transaction

#endif // ELASTICSEARCH_AUX_ALL_REQUESTS_HPP
