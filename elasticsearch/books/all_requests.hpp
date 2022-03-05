#ifndef ELASTICSEARCH_BOOKS_ALL_REQUESTS_HPP
#define ELASTICSEARCH_BOOKS_ALL_REQUESTS_HPP

#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/request/create_book.hpp"
#include "elasticsearch/books/request/delete_book.hpp"
#include "elasticsearch/books/request/get_book.hpp"
#include "elasticsearch/books/request/search_book.hpp"

#define ALL_BOOKS_REQUEST_LIST                                                                      \
                            elasticsearch::book::index_mapping::transaction,                        \
                            elasticsearch::book::create::transaction,                               \
                            elasticsearch::book::del::transaction,                                  \
                            elasticsearch::book::get::transaction,                                  \
                            elasticsearch::book::search::transaction

#endif // ELASTICSEARCH_BOOKS_ALL_REQUESTS_HPP
