cmake_minimum_required(VERSION 3.10.0)
project(levelapi_server VERSION 1.0.0)

set(CXX_STANDARD_REQUIRED 17)
set(CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

option(LAPI_COMPILE_LAUNCHER "compile levelapi launcher" ON)
option(LAPI_COMPILE_TEST "compile test executables" ON)
option(LAPI_COMPILE_ADDITIONAL "compile additional executables" ON)
option(LAPI_STATIC "compile everything as static libraries" OFF)
option(LAPI_ENABLE_HTTP "enable internal http server" ON)
option(LAPI_ENABLE_DISCORD "enable discord support" ON)

if (LAPI_STATIC)
    set(COMPILE_MODE STATIC)
else()
    set(COMPILE_MODE SHARED)
endif()

# add_library(httpserver
#     libhttpserver/src/basic_auth_fail_response.cpp
#     libhttpserver/src/deferred_response.cpp
#     libhttpserver/src/digest_auth_fail_response.cpp
# )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (LAPI_ENABLE_HTTP)
    file(GLOB httpserver_src CONFIGURE_DEPENDS
        "libhttpserver/src/*.hpp"
        "libhttpserver/src/*.h"
        "libhttpserver/src/*.cpp"

        "libhttpserver/src/details/*.cpp"

        "libhttpserver/src/httpserver/*.hpp"
        "libhttpserver/src/httpserver/details/*.hpp"
    )
    file(GLOB levelapi_server_requests CONFIGURE_DEPENDS
        "requests/*.cpp"
        "requests/*.h"
    )
    set (HTTP_LINKAGE PUBLIC httpserver)
else()
    set (httpserver_src "")
    set (levelapi_server_requests "")
    set (HTTP_LINKAGE "")
endif()
set(levelapi_splitstring
    StringSplit.cpp StringSplit.h
)
set(levelapi_sqlite
    SQLiteManager.cpp SQLiteManager.h
)
set(levelapi_robtopstring
    RobTopStringContainer.cpp RobTopStringContainer.hpp
)
set(iandy_src
    iAndy.cpp iAndy.h
)

set(levelapi_baselibrary
    Account10.cpp Account10.h
    Account19.cpp Account19.h
    Account21.cpp Account19.h
    curl_backend.h CURLConnection.cpp CURLConnectionSettings.cpp CURLParameter.cpp CURLProxy.cpp
    DatabaseCell.cpp DatabaseCell.h
    DatabaseController.cpp DatabaseControllerThreads.h
    DatabaseRunner.cpp
    GDPSActionLog.h
    GDServer.cpp GDServer.h
    GDServer_BoomlingsLike19.cpp GDServer_BoomlingsLike19.h
    GDServer_BoomlingsLike21.cpp GDServer_BoomlingsLike21.h
    GDServer_BoomlingsLike22.cpp GDServer_BoomlingsLike22.h
    GDServer_BoomlingsBase.cpp GDServer_BoomlingsBase.h
    GDServer_Boomlings21.cpp GDServer_Boomlings21.h
    GDServer_Boomlings22.cpp GDServer_Boomlings22.h
    GDServer_BasementLike21.cpp GDServer_BasementLike21.h
    GDServer_BasementLike22.cpp GDServer_BasementLike22.h
    GDServer_BasementGDPS.cpp GDServer_BasementGDPS.h
    GDServer_19GDPS.h GDServer_19GDPS.cpp
    GDServer_NelisPS.h GDServer_NelisPS.cpp
    gmd.cpp gmd.h
    Level.cpp Level.h
    LevelRange.cpp LevelRange.h
    LevelRangeList.cpp LevelRangeList.h
    LevelRelease.cpp LevelRelease.h
    ModuleGDHistory.cpp ModuleGDHistory.h
    NodeRunner.cpp NodeRunner.h
    NodeRunnerLegacy.cpp
    lapi_version.h
    lapi_database.h Node.cpp NodeCommandQueue.cpp NodeDatabase.cpp NodePolicy.cpp NodeProxyList.cpp NodeQueue.cpp Database.cpp
    SearchFilter.h
    ServerModule.cpp
    ServerModule.h
    SmallLevel.cpp SmallLevel.h
    tests.h test_curl_connection.cpp test_gd_parsers.cpp test_basement_features.cpp
    tasks.h task_find_list_levels.cpp
    TestingBoomlings22.cpp TestingBoomlings22.h
    ThreadSafeLevelParser.cpp ThreadSafeLevelParser.h
    Time.cpp Time.h
    Tools.h
    Translation.cpp
    Translation.h
    UUID.cpp UUID.h
    ModuleEnabled.cpp ModuleEnabled.h
    PixelCharacters.cpp PixelCharacters.hpp
    test_robtop_parser.cpp
)

if (LAPI_ENABLE_HTTP)
    include_directories(httpserver
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
    add_compile_definitions(_HTTPSERVER_HPP_INSIDE_)
    add_library(httpserver ${COMPILE_MODE} ${httpserver_src})
    target_link_libraries(httpserver PUBLIC microhttpd)
    install(TARGETS httpserver EXPORT levelapi_workarounds)
    set(levelapi_http_support
        HttpController.cpp HttpController.h
    )
else()
    set(levelapi_http_support "")
endif()

set(FMT_INSTALL OFF)

if (LAPI_STATIC)
    option(BUILD_SHARED_LIBS OFF)
    set(BUILD_SHARED_LIBS OFF)
else()
    option(BUILD_SHARED_LIBS ON)
    set(BUILD_SHARED_LIBS ON)
endif()
add_subdirectory(fmt)

# install(TARGETS fmt EXPORT levelapi_workarounds)

if (LAPI_STATIC)
    option(GMD2PP_STATIC ON)
    set(GMD2PP_STATIC ON)
    option(GDCRYPTO_STATIC ON)
    set(GDCRYPTO_STATIC ON)
    option(BUILD_SHARED_LIBS OFF)
    set(BUILD_SHARED_LIBS OFF)
endif()
add_subdirectory(gmd2pp)
add_subdirectory(GDCrypto-patched)
if (LAPI_ENABLE_DISCORD)
    set(DPP_BUILD_TEST OFF)
    option(DPP_BUILD_TEST OFF)
    add_subdirectory(DPP)
    set(DPP_LINKAGE PUBLIC dpp)
    add_compile_definitions(_DPP_ENABLED_)
    set(levelapi_dpp_support
        DCommand.cpp DCommand.h
        DCommandEvent.cpp DCommandEvent.h
        DCommandSearch.cpp DCommandSearch.h
        DCommandStats.cpp DCommandStats.h
        DiscordInstance.cpp DiscordInstance.h
    )
else()
    set(DPP_LINKAGE "")
    set(levelapi_dpp_support "")
endif()

option(CUSTOMIZE_BUILD ON)
option(SUPPORT_MODULE_RSHAPES OFF)
option(SUPPORT_MODULE_RMODELS OFF)
option(SUPPORT_MODULE_RAUDIO OFF)
option(SUPPORT_CAMERA_SYSTEM OFF)
option(SUPPORT_GESTURES_SYSTEM OFF)
option(SUPPORT_MOUSE_GESTURES OFF)
option(SUPPORT_SSH_KEYBOARD_RPI OFF)
option(SUPPORT_SCREEN_CAPTURE OFF)
option(SUPPORT_GIF_RECORDING OFF)
option(SUPPORT_BUSY_WAIT_LOOP OFF)
option(USE_AUDIO OFF)
option(USE_WAYLAND OFF)
option(USE_EXTERNAL_GLFW IF_POSSIBLE)
option(GLFW_BUILD_WAYLAND OFF)
if (LAPI_STATIC)
    option(BUILD_SHARED_LIBS OFF)
else()
    option(BUILD_SHARED_LIBS ON)
endif()

add_subdirectory(raylib)

#set(BUILD_SQLITE3_CONNECTOR ON)
#add_subdirectory(sqlpp11)

install(TARGETS gmd2 EXPORT levelapi_workarounds)
install(EXPORT levelapi_workarounds DESTINATION "./")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(sqlitemanager ${COMPILE_MODE}
    ${levelapi_sqlite}
)

add_library(robtopstrings ${COMPILE_MODE}
    ${levelapi_robtopstring}
)

add_library(splitstring ${COMPILE_MODE}
    ${levelapi_splitstring}
)

add_library(iandy ${COMPILE_MODE}
    ${iandy_src}
)

target_link_libraries(sqlitemanager PUBLIC sqlite3)
target_link_libraries(robtopstrings PUBLIC splitstring)

add_library(levelapi ${COMPILE_MODE}
    ${levelapi_baselibrary}
    ${levelapi_server_requests}
    ${levelapi_http_support}
    ${levelapi_dpp_support}
)
target_include_directories(levelapi PRIVATE
    GDCrypto-patched/Include
    fmt/include
    raylib/src
)
if (LAPI_ENABLE_DISCORD)
    target_include_directories(levelapi PRIVATE
        DPP/include
    )
endif()

add_executable(test_mkdir test_mkdir.c)

add_library(levelapi-generic-tools ${COMPILE_MODE}
    "GenericTools.cpp" "GenericTools.hpp"
)

target_link_libraries(levelapi
    ${HTTP_LINKAGE}
    PUBLIC gmd2
    ${DPP_LINKAGE}
    PUBLIC fmt
    PUBLIC GDCrypto
    PUBLIC sqlitemanager
    PUBLIC robtopstrings
    PUBLIC iandy
    PUBLIC raylib
    PUBLIC levelapi-generic-tools
)

if (LAPI_COMPILE_TEST)
    add_executable(levelapi_test_folders
        "ExternalTest_Folders.cc"
    )
endif()
if (LAPI_COMPILE_ADDITIONAL)
    add_executable(levelapi-migrate-folders
        "levelapi_migrate_folders.cpp"
    )
    target_link_libraries(levelapi-migrate-folders levelapi-generic-tools)
endif()

file(COPY resources DESTINATION .)

if (LAPI_COMPILE_LAUNCHER)
    add_executable(levelapi_server
        main.cpp
    )
    target_link_libraries(levelapi_server PUBLIC levelapi)
endif()

message(STATUS "")
message(STATUS "")
message(STATUS "     LevelAPI Compiler Settings")
message(STATUS "Compiling launcher: ${LAPI_COMPILE_LAUNCHER}")
message(STATUS "Compiling test targets: ${LAPI_COMPILE_TEST}")
message(STATUS "Compiling additional targets: ${LAPI_COMPILE_ADDITIONAL}")
message(STATUS "Compiling in static mode: ${LAPI_STATIC}")
message(STATUS "Compiling with Discord support: ${LAPI_ENABLE_DISCORD}")
message(STATUS "Compiling with HTTP (webserver) support: ${LAPI_ENABLE_HTTP}")
message(STATUS "")
message(STATUS "")
