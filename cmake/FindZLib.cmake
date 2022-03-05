if(WITH_ZLIB)
    find_package(PkgConfig REQUIRED)
    set (PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
    if (NOT ZLIB_INSTALLED_PATH)
        pkg_search_module(ZLIB QUIET IMPORTED_TARGET zlib)
    else()
        list(APPEND CMAKE_PREFIX_PATH ${ZLIB_INSTALLED_PATH})
        message("Search ZLib in CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
        #find_package(CURL QUIET PATHS ${ZLIB_INSTALLED_PATH} NO_DEFAULT_PATH)
        pkg_search_module(ZLIB QUIET IMPORTED_TARGET zlib)
    endif()

    if (NOT TARGET zlib)
        message("Download ZLib from repositories")
        configure_file(${BRANCH_ROOT}/cmake/download_zlib.cmake.in zlib-download/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/zlib-download )
        if(result)
            message(FATAL_ERROR "CMake step for ZLib failed: ${result}. Log: ${output}")
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} --build . --parallel 4
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/zlib-download )
        if(result)
            message(FATAL_ERROR "Build step for ZLib failed: ${result}. Log: ${output}")
        endif()
        message("ZLib Build Log: ${output}")

        pkg_search_module(ZLIB REQUIRED IMPORTED_TARGET zlib)

        list (APPEND COMPILE_DEFS -DWITH_ZLIB)
        set (FILE_ARCHIVER_TARGET PkgConfig::ZLIB)
    else()
        set (FILE_ARCHIVER_TARGET PkgConfig::ZLIB)
        list (APPEND COMPILE_DEFS -DWITH_ZLIB)
    endif()
endif()
