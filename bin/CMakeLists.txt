cmake_minimum_required (VERSION 3.0)

set (ES_BIN_VERSION     v7)

set (COMMON_SRC         utils/cfg_reader.cpp
                        utils/parse_string.cpp
                        ${ES_BIN_VERSION}/dispatcher.cpp
                        ${ES_BIN_VERSION}/cfg_param_names.cpp)

file (GLOB BINARY_SRC *.cpp)
foreach (file ${BINARY_SRC})
    get_filename_component(filename ${file} NAME_WLE)
    get_filename_component(dirname ${file} DIRECTORY)

    add_executable(${filename} ${filename}.cpp ${COMMON_SRC})
    target_link_libraries(${filename} PRIVATE
                        ${ELASTICSEARCH_APPLICATION_TARGET}
                        gtest_main)
    target_include_directories(${filename}  PUBLIC
                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../>)

    target_compile_features(${filename} PUBLIC cxx_std_17)
endforeach (file ${BINARY_SRC})
