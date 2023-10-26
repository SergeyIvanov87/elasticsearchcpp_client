cmake_minimum_required (VERSION 3.6)

if(WITH_CIVETWEB)
    set(CIVETWEB_PACKAGE_TARGET civetweb::civetweb)
#    set (PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
    if (NOT CIVETWEB_INSTALLED_PATH)
        find_package(civetweb QUIET)
    else()
        #list(APPEND CMAKE_PREFIX_PATH ${CIVETWEB_INSTALLED_PATH})
        message("Search CIVETWEB_PACKAGE_TARGET in CIVETWEB_INSTALLED_PATH: ${CIVETWEB_INSTALLED_PATH}")
        find_package(civetweb QUIET PATHS ${CIVETWEB_INSTALLED_PATH} NO_DEFAULT_PATH)
    endif()

    if (NOT TARGET ${CIVETWEB_PACKAGE_TARGET})
        message("Download CIVETWEB_PACKAGE_TARGET from repositories")

        if (NOT CIVETWEB_INSTALLED_PATH)
            set (CIVETWEB_INSTALLED_PATH "install")
        endif()

        configure_file(${BRANCH_ROOT}/cmake/download_civetweb.cmake.in civetweb/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/civetweb )
        if(result)
            message(FATAL_ERROR "CMake step for CIVETWEB_PACKAGE_TARGET failed: ${result}. Log: ${output}")
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} --build . --parallel 4
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/civetweb )
        if(result)
            message(FATAL_ERROR "Build step for CIVETWEB_PACKAGE_TARGET failed: ${result}. Log: ${output}")
        endif()
        message("CIVETWEB_PACKAGE_TARGET Build Log: ${output}")

        set(CIVETWEB_INSTALLED_PATH "${CMAKE_CURRENT_BINARY_DIR}/civetweb/${CIVETWEB_INSTALLED_PATH}")
        find_package(civetweb REQUIRED PATHS ${CIVETWEB_INSTALLED_PATH}  NO_DEFAULT_PATH)

        list (APPEND COMPILE_DEFS -DWITH_CIVETWEB)
        set (CIVETWEB_TARGET ${CIVETWEB_PACKAGE_TARGET})
    else()
        set (CIVETWEB_TARGET ${CIVETWEB_PACKAGE_TARGET})
        list (APPEND COMPILE_DEFS -DWITH_CIVETWEB)
    endif()
endif()
message("CIVETWEB_PACKAGE_TARGET has been bootstrapped and the package resides in: ${CIVETWEB_INSTALLED_PATH}")
