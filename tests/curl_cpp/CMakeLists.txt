cmake_minimum_required (VERSION 3.0)

file (GLOB SUITE_SRC *.cpp)
foreach (file ${SUITE_SRC})
    get_filename_component(filename ${file} NAME_WLE)
    get_filename_component(dirname ${file} DIRECTORY)

    add_executable(${filename} ${filename}.cpp)
    target_link_libraries(${filename}
                                    PRIVATE
                                    ${CURL_CPP_WRAPPER_TARGET}
                                    gtest_main)
    target_include_directories(${filename}  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../>)

    target_compile_features(${filename}  PUBLIC cxx_std_17)
endforeach (file ${SUITE_SRC})
