cmake_minimum_required(VERSION 3.0.0)
project(levelapi_server VERSION 1.0.0)

# add_library(httpserver
#     libhttpserver/src/basic_auth_fail_response.cpp
#     libhttpserver/src/deferred_response.cpp
#     libhttpserver/src/digest_auth_fail_response.cpp
# )

file(GLOB httpserver_src
    "libhttpserver/src/*.hpp"
    "libhttpserver/src/*.h"
    "libhttpserver/src/*.cpp"

    "libhttpserver/src/details/*.cpp"

    "libhttpserver/src/httpserver/*.hpp"
    "libhttpserver/src/httpserver/details/*.hpp"
)

file(GLOB levelapi_server_requests
    "requests/*.cpp"
    "requests/*.h"
)

include_directories(httpserver PRIVATE
    libhttpserver/src
    libhttpserver/src/details
    libhttpserver/src/httpserver
    libhttpserver/src/httpserver/details
)

add_compile_definitions(_HTTPSERVER_HPP_INSIDE_)
add_library(httpserver ${httpserver_src})
link_libraries(httpserver microhttpd)

set(BUILD_MYSQL_CONNECTOR ON)
set(BUILD_MARIADB_CONNECTOR ON)
set(BUILD_SQLITE3_CONNECTOR ON)
# set(BUILD_POSTGRESQL_CONNECTOR ON)
add_subdirectory(sqlpp11)

add_subdirectory(gmd2pp)

add_executable(levelapi_server 
    main.cpp
    ${levelapi_server_requests}
)
target_link_libraries(levelapi_server httpserver sqlpp11)