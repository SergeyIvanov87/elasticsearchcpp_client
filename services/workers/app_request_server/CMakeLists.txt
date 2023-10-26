# configure Web Server
set(WITH_CIVETWEB   ON CACHE BOOL "Configure with CIVETWEB")

include(${BRANCH_ROOT}/cmake/FindCIVETWEB.cmake)

set(WEB_SERVER_TARGET ${CIVETWEB_TARGET})
add_subdirectory(books_pusher)
