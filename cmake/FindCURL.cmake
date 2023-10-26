cmake_minimum_required (VERSION 3.6)

if(WITH_CURL)
#    set (PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
    if (NOT CURL_INSTALLED_PATH)
        find_package(CURL QUIET)
    else()
        #list(APPEND CMAKE_PREFIX_PATH ${CURL_INSTALLED_PATH})
        message("Search CURL in CURL_INSTALLED_PATH: ${CURL_INSTALLED_PATH}")
        find_package(CURL QUIET PATHS ${CURL_INSTALLED_PATH} NO_DEFAULT_PATH)
    endif()

    if (NOT TARGET CURL::libcurl)
        message("Download CURL from repositories")

        if (NOT CURL_INSTALLED_PATH)
            set (CURL_INSTALLED_PATH "install")
        endif()

        configure_file(${BRANCH_ROOT}/cmake/download_curl.cmake.in curl/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/curl )
        if(result)
            message(FATAL_ERROR "CMake step for CURL failed: ${result}. Log: ${output}")
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} --build . --parallel 4
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/curl )
        if(result)
            message(FATAL_ERROR "Build step for CURL failed: ${result}. Log: ${output}")
        endif()
        message("CURL Build Log: ${output}")

        set(CURL_INSTALLED_PATH "${CMAKE_CURRENT_BINARY_DIR}/curl/${CURL_INSTALLED_PATH}")
        find_package(CURL REQUIRED PATHS ${CURL_INSTALLED_PATH}  NO_DEFAULT_PATH)

        list (APPEND COMPILE_DEFS -DWITH_CURL)
        set (CURL_TARGET CURL::libcurl)
    else()
        set (CURL_TARGET CURL::libcurl)
        list (APPEND COMPILE_DEFS -DWITH_CURL)
    endif()
endif()
message("CURL has been bootstrapped and the package resides in: ${CURL_INSTALLED_PATH}")
