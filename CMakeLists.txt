cmake_minimum_required (VERSION 3.1)

set (PROC_NAME "BookIndexer")
set (PROC_LIB_NAME "cpp_book_indexer")

project(${PROC_NAME} CXX C)

set (BRANCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(BRANCH_ROOT ${BRANCH_ROOT} ABSOLUTE)

set (PROCESS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(PROCESS_DIR ${PROCESS_DIR} ABSOLUTE)

set (BRANCH_NAME ${PROC_NAME})

set(WITH_CURL       ON CACHE BOOL "Configure with CURL")
set(WITH_TXML       ON CACHE BOOL "Configure with TemplatedXML")
set(WITH_JSON       ON CACHE BOOL "Configure with JSON")
set(WITH_ZLIB       ON CACHE BOOL "Configure with ZLib")
set(WITH_EASYEXIF   ON CACHE BOOL "Configure with EasyExif")

set(WITH_UT     ON CACHE BOOL "Configure with UnitTests")
set(WITH_BIN    ON CACHE BOOL "Configure with binary utilites")


if (WITH_CURL)
    include(${BRANCH_ROOT}/cmake/FindCURL.cmake)
endif()

if (WITH_TXML)
    include(${BRANCH_ROOT}/cmake/FindTXML.cmake)
endif()

if (WITH_JSON)
    include(${BRANCH_ROOT}/cmake/FindJSON.cmake)
endif()

if (WITH_ZLIB)
    include(${BRANCH_ROOT}/cmake/FindZLib.cmake)
    list(APPEND ZLIB_CONTRIB_SRC
                    zlib/contrib/minizip/ioapi.c
                    #zlib/contrib/minizip/miniunz.c
                    zlib/contrib/minizip/unzip.c
                    zlib/contrib/minizip/zip.c
        )
    list(APPEND ZLIB_CONTRIB_INCL
                    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/contrib/minizip)
endif()

if (WITH_EASYEXIF)
    include(${BRANCH_ROOT}/cmake/FindEasyExif.cmake)
    list(APPEND EASYEXIF_SRC
                    ${EASYEXIF_PATH}/exif.cpp
        )
    list(APPEND EASYEXIF_INCL
                    ${EASYEXIF_PATH})
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -ftemplate-backtrace-limit=0")
set (CURL_CPP_WRAPPER_TARGET curl_cpp_wrapper)
list (APPEND CURL_CPP_WRAPPER_SRC
                        sources/http_client/http_connection.cpp
                        sources/http_client/http_get.cpp

                        sources/adapter/curlu/url_part_adapter.cpp

                        sources/adapter/easy/POST/CopyPOSTFields.cpp
                        sources/adapter/easy/POST/POSTFields.cpp
                        sources/adapter/easy/POST/POSTUpload.cpp

                        sources/adapter/easy/curlu_adapter.cpp
                        sources/adapter/easy/multi_adapter.cpp
                        sources/adapter/easy/post_adapter.cpp
                        sources/adapter/easy/sstream_receiver.cpp
                        sources/adapter/easy/upload_adapter.cpp
                        sources/adapter/easy/custom_request_adapter.cpp
                        sources/adapter/easy/http/custom_header_adapter.cpp

                        sources/interface_impl/upload_data_provider/binary_array_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/string_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/istream_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/istringstream_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/stringstream_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/string_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/json_upload_data_provider.cpp
                        sources/interface_impl/upload_data_provider/data_provider_dispatcher.cpp

                        sources/connection/connection.cpp
                        sources/connection/multi_connection.cpp)
add_library(${CURL_CPP_WRAPPER_TARGET}
                        SHARED
                        ${CURL_CPP_WRAPPER_SRC})
target_link_libraries(${CURL_CPP_WRAPPER_TARGET} ${CURL_TARGET} ${JSON_TARGET})
target_compile_definitions(${CURL_CPP_WRAPPER_TARGET} PUBLIC -DHAVE_JSON)

#set_target_properties(${CURL_CPP_WRAPPER_TARGET}  PROPERTIES PUBLIC_HEADER sources/)
target_include_directories(${CURL_CPP_WRAPPER_TARGET}
                        PUBLIC
                        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/sources>"
                        "$<INSTALL_INTERFACE:./;sources>")

set (ELASTICSEARCH_CLIENT_TARGET es_client)
list (APPEND ELASTICSEARCH_CLIENT_SRC
                        elasticsearch/v7_10/request/index_mapping.cpp
                        elasticsearch/v7_10/request/index_mapping_delete.cpp
                        elasticsearch/v7_10/request/get_data.cpp
                        elasticsearch/v7_10/request/put_data.cpp
                        elasticsearch/v7_10/request/delete_data.cpp
                        elasticsearch/v7_10/request/create_pit.cpp
                        elasticsearch/v7_10/request/delete_pit.cpp
                        elasticsearch/v7_10/request/search.cpp
                        elasticsearch/v7_10/request/base/base_request.cpp
                        elasticsearch/v7_10/answer_model/object/utils.cpp
                        elasticsearch/utils/base64.cpp
                        elasticsearch/utils/strings.cpp
                        elasticsearch/utils/file_to_base64_packer.cpp
                        elasticsearch/utils/base64_to_file_unpacker.cpp

                        elasticsearch/aux/doc_id_pm.cpp
                        elasticsearch/aux/doc_id_create.cpp

                        elasticsearch/utils/zutils.cpp)
add_library(${ELASTICSEARCH_CLIENT_TARGET}
                        STATIC
                        ${ELASTICSEARCH_CLIENT_SRC})
target_link_libraries(${ELASTICSEARCH_CLIENT_TARGET}
                        PUBLIC
                        ${CURL_CPP_WRAPPER_TARGET}
                        ${TXML_TARGET}
                        ${JSON_TARGET})

target_include_directories(${ELASTICSEARCH_CLIENT_TARGET}
                        PRIVATE
                        .)
target_include_directories(${ELASTICSEARCH_CLIENT_TARGET}
                        PUBLIC
                        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/elasticsearch>"
                        "$<INSTALL_INTERFACE:./;elasticsearch>")


set (ELASTICSEARCH_APPLICATION_TARGET es_application)
list (APPEND ELASTICSEARCH_APPLICATION_SRC
                        elasticsearch/books/request/create_book.cpp
                        elasticsearch/books/request/get_book.cpp
                        elasticsearch/books/request/delete_book.cpp
                        elasticsearch/books/request/book_index_mapping.cpp
                        elasticsearch/books/request/search_book.cpp

                        elasticsearch/books/EPUB/epub_reader.cpp
                        elasticsearch/books/FB2/fb2_packer.cpp
                        elasticsearch/books/FB2/fb2_unpacker.cpp
                        elasticsearch/books/FB2/fb2_reader.cpp
                        elasticsearch/books/book_reader.cpp

                        elasticsearch/images/request/create_image.cpp
                        elasticsearch/images/request/get_image.cpp
                        elasticsearch/images/request/delete_image.cpp
                        elasticsearch/images/request/image_index_mapping.cpp
                        elasticsearch/images/request/search_image.cpp

                        elasticsearch/images/JPG/reader.cpp
                        elasticsearch/images/data_model/GeoPoint.cpp
                        elasticsearch/images/data_model/Resolution.cpp
                        elasticsearch/images/image_reader.cpp)


add_library(${ELASTICSEARCH_APPLICATION_TARGET}
                        STATIC
                        ${ELASTICSEARCH_APPLICATION_SRC}
                        ${ZLIB_CONTRIB_SRC}
                        ${EASYEXIF_SRC})
target_link_libraries(${ELASTICSEARCH_APPLICATION_TARGET}
                        PUBLIC
                        ${ELASTICSEARCH_CLIENT_TARGET}
                        ${FILE_ARCHIVER_TARGET})
target_include_directories(${ELASTICSEARCH_APPLICATION_TARGET}
                        PRIVATE
                        .
                        ${ZLIB_CONTRIB_INCL}
                        ${EASYEXIF_INCL})

if (WITH_UT)
    add_subdirectory(tests)
endif()

if (WITH_BIN)
    add_subdirectory(bin)
endif()
#add_executable(test sources/main.cpp)# ${ELASTICSEARCH_CLIENT} ${ELASTICSEARCH_APPLICATION})
#target_include_directories(test PUBLIC
#            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/sources>;${INCL}"
#            "$<INSTALL_INTERFACE:./;sources>"
#            )

#target_link_libraries(test  ${ELASTICSEARCH_APPLICATION_TARGET})
