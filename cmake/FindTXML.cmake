cmake_minimum_required (VERSION 3.6)

set(WITH_XML2 ON CACHE BOOL "TXml with XML2")

if(WITH_TXML)
    if (NOT TEMPLATE_XML_PATH)
        find_package(TXml QUIET)
        # TODO cmake for tXml helpers is in different folder but not in the TXml package
        if (TXml_FOUND)
            cmake_path(REMOVE_FILENAME TXml_CONFIG OUTPUT_VARIABLE TXml_PACKAGE_PATH)
            cmake_path(APPEND TXml_PACKAGE_PATH ..)
            cmake_path(APPEND TXml_PACKAGE_PATH ..)
            cmake_path(APPEND TXml_PACKAGE_PATH ..)
            set (TEMPLATE_XML_PATH ${TXml_PACKAGE_PATH})
        endif()
    else()
        message("Search TemplateXML in TEMPLATE_XML_PATH: ${TEMPLATE_XML_PATH}")
        find_package(TXml QUIET PATHS ${TEMPLATE_XML_PATH} NO_DEFAULT_PATH)
    endif()

    if (NOT TARGET TXml)
        message("Download TemplateXML from repositories")

        if (NOT TEMPLATE_XML_PATH)
            set (TEMPLATE_XML_PATH "install")
        endif()
        configure_file(${BRANCH_ROOT}/cmake/download_txml.cmake.in templatexml/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/templatexml )
        if(result)
            message(FATAL_ERROR "CMake step for TemplateXML failed: ${result}. Log: ${output}")
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} --build .
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE output
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/templatexml )
        if(result)
            message(FATAL_ERROR "Build step for TemplateXML failed: ${result}. Log: ${output}")
        endif()
        message("TemplateXML Build Log: ${output}")

        set(TEMPLATE_XML_PATH "${CMAKE_CURRENT_BINARY_DIR}/templatexml/${TEMPLATE_XML_PATH}")
        find_package(TXml REQUIRED PATHS ${TEMPLATE_XML_PATH} NO_DEFAULT_PATH)
        list (APPEND COMPILE_DEFS -DWITH_TXML)
        set (TXML_TARGET TXml)
    else()
        set (TXML_TARGET TXml)
        list (APPEND COMPILE_DEFS -DWITH_TXML)
    endif()
endif()

include(${TEMPLATE_XML_PATH}/lib/txml/cmake/FindLibXML2.cmake)
target_link_libraries(${TXML_TARGET} INTERFACE ${XML_TARGET})
add_dependencies(${TXML_TARGET} ${XML_TARGET})
message("TemplateXML has been bootstrapped and the package resides in: ${TEMPLATE_XML_PATH}")
