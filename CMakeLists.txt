cmake_minimum_required(VERSION 3.27)
project(PI2 C)

set(CMAKE_C_STANDARD 11)

add_executable(PI2 main.c)

find_package(PostgreSQL REQUIRED)
include_directories("C:/Program Files/PostgreSQL/16/include")
include_directories("C:/Program Files/PostgreSQL/16/include/libpq-fe.h")
target_link_libraries(PI2 PRIVATE PostgreSQL::PostgreSQL)
target_link_libraries(PI2 PRIVATE "C:/Program Files/PostgreSQL/16/include/libpq-fe.h")
