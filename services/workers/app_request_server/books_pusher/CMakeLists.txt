project(books_pusher CXX)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${WEB_SERVER_TARGET})
