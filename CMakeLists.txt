cmake_minimum_required(VERSION 3.10.0)
project(levelapi_server VERSION 1.0.0)

set(CXX_STANDARD_REQUIRED 17)
set(CXX_STANDARD 17)

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
    DCommand.cpp DCommand.h
    DCommandEvent.cpp DCommandEvent.h
    DCommandSearch.cpp DCommandSearch.h
    DCommandStats.cpp DCommandStats.h
    DiscordInstance.cpp DiscordInstance.h
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
    gmd.cpp gmd.h
    HttpController.cpp HttpController.h
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
#include_directories(levelapi_server
#    sqlpp11/include
#    sqlpp11/include/sqlpp11
#    # sqlpp11/include/sqlpp11/sqlite3
#)

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
option(USE_WAYLAND OFF)
option(USE_EXTERNAL_GLFW IF_POSSIBLE)
option(GLFW_BUILD_WAYLAND OFF)

add_subdirectory(raylib)

#set(BUILD_SQLITE3_CONNECTOR ON)
#add_subdirectory(sqlpp11)

install(TARGETS gmd2 EXPORT levelapi_workarounds)
install(EXPORT levelapi_workarounds DESTINATION "./")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(sqlitemanager
    ${levelapi_sqlite}
)

add_library(robtopstrings
    ${levelapi_robtopstring}
)

add_library(splitstring
    ${levelapi_splitstring}
)

add_library(iandy
    ${iandy_src}
)

target_link_libraries(sqlitemanager sqlite3)
target_link_libraries(robtopstrings splitstring)

add_library(levelapi
    ${levelapi_baselibrary}
    ${levelapi_server_requests}
)

add_executable(test_mkdir test_mkdir.c)

add_library(levelapi-generic-tools
    "GenericTools.cpp" "GenericTools.hpp"
)

target_link_libraries(levelapi httpserver gmd2 dpp fmt GDCrypto sqlitemanager robtopstrings iandy raylib levelapi-generic-tools)

add_executable(levelapi_server 
    main.cpp
)
add_executable(levelapi_test_folders
    "ExternalTest_Folders.cc"
)
add_executable(levelapi-migrate-folders
    "levelapi_migrate_folders.cpp"
)

file(COPY resources DESTINATION .)

target_link_libraries(levelapi_server levelapi)
target_link_libraries(levelapi-migrate-folders levelapi-generic-tools)
