cmake_minimum_required(VERSION 3.5.0)
project(levelapi_server VERSION 1.0.0)

set(CXX_STANDARD_REQUIRED 17)

# add_library(httpserver
#     libhttpserver/src/basic_auth_fail_response.cpp
#     libhttpserver/src/deferred_response.cpp
#     libhttpserver/src/digest_auth_fail_response.cpp
# )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
file(GLOB levelapi_base
    "*.cpp"
    "*.h"
)

include_directories(httpserver PRIVATE
    libhttpserver/src
    libhttpserver/src/details
    libhttpserver/src/httpserver
    libhttpserver/src/httpserver/details
    DPP/include/dpp
    DPP/include/
    fmt/inlcude
    fmt/include/fmt
    GDCrypto-patched/Include
    GDCrypto-patched/Include/GDCrypto
)
# include_directories(levelapi_server PRIVATE

# )

add_compile_definitions(_HTTPSERVER_HPP_INSIDE_)
add_library(httpserver ${httpserver_src})
link_libraries(httpserver microhttpd)

set(FMT_INSTALL OFF)

add_subdirectory(fmt)

# install(TARGETS fmt EXPORT levelapi_workarounds)

install(TARGETS httpserver EXPORT levelapi_workarounds)

add_subdirectory(gmd2pp)
add_subdirectory(GDCrypto-patched)
add_subdirectory(DPP)

install(TARGETS gmd2 EXPORT levelapi_workarounds)
install(EXPORT levelapi_workarounds DESTINATION "./")

add_executable(levelapi_server 
    ${levelapi_base}
    ${levelapi_server_requests}
)
add_executable(levelapi_test_folders
    "ExternalTest_Folders.cc"
)

file(COPY images DESTINATION .)

target_link_libraries(levelapi_server httpserver gmd2 dpp fmt opencv_core opencv_imgcodecs opencv_imgproc GDCrypto)