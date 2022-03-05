#ifndef ELASTICSEARCH_IMAGES_ALL_REQUESTS_HPP
#define ELASTICSEARCH_IMAGES_ALL_REQUESTS_HPP

#include "elasticsearch/images/request/image_index_mapping.hpp"
#include "elasticsearch/images/request/create_image.hpp"
#include "elasticsearch/images/request/delete_image.hpp"
#include "elasticsearch/images/request/get_image.hpp"
#include "elasticsearch/images/request/search_image.hpp"

#define ALL_IMAGES_REQUEST_LIST                                                                     \
                            elasticsearch::image::index_mapping::transaction,                       \
                            elasticsearch::image::create::transaction,                              \
                            elasticsearch::image::del::transaction,                                 \
                            elasticsearch::image::get::transaction,                                 \
                            elasticsearch::image::search::transaction

#endif // ELASTICSEARCH_IMAGES_ALL_REQUESTS_HPP
