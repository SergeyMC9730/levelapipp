cmake_minimum_required(VERSION 3.5.0)
project(gmd2 VERSION 1.0.0)

option(GMD2PP_STATIC "compile gmd2pp as a static library" OFF)

if (GMD2PP_STATIC)
    set(COMPILE_MODE STATIC)
else()
    set(COMPILE_MODE SHARED)
endif()

file(GLOB zlib_files
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/zlib/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/zlib/*.h"
)
file(GLOB bzip2_files
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/bzip2/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/bzip2/*.h"
)

if (WIN32)
    file(GLOB lzma_files
        "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/*.h"
    )
else()
    file(GLOB lzma_files
        "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/unix/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/unix/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/unix/LzmaUtil/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/unix/LzmaUtil/*.h"
    )
endif()

if (WIN32)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma")
else()
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/lzma/unix")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/bzip2")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/extlibs/zlib")

add_library(ext_bzip2 ${COMPILE_MODE} ${bzip2_files})
add_library(ext_lzma ${COMPILE_MODE} ${lzma_files})
add_library(ext_zlib ${COMPILE_MODE} ${zlib_files}) 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(gmd2pp_helper_hex
    hex.cpp hex.h
)

file(GLOB ziplib_files
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/utils/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/streams/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/streams/streambuffs/*.h"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/methods/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/detail/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/detail/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/bzip2/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/deflate/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/lzma/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/lzma/detail/*.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/lzma/*.h"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/store/*.h"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/streams"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/streams/streambuffs"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/methods"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/detail"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/bzip2"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/deflate"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/lzma"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/lzma/detail"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/lzma"
    "${CMAKE_CURRENT_SOURCE_DIR}/ziplib/Source/ZipLib/compression/store"
)

add_library(ext_ziplib ${COMPILE_MODE} ${ziplib_files})
link_libraries(ext_ziplib ext_bzip2 ext_lzma ext_zlib)

 #ext_bzip2 ext_lzma ext_zlib

# install(TARGETS ext_bzip2 ext_lzma ext_zlib ext_ziplib )

install(TARGETS ext_bzip2 EXPORT levelapi_workarounds)
install(TARGETS ext_lzma EXPORT levelapi_workarounds)
install(TARGETS ext_zlib EXPORT levelapi_workarounds)
install(TARGETS ext_ziplib EXPORT levelapi_workarounds)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_library(gmdtools ${COMPILE_MODE} gmdtools.cpp)
link_libraries(gmdtools curl)

install(TARGETS gmdtools EXPORT levelapi_workarounds)

if (GMD2PP_STATIC)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs" FORCE)
    option(BUILD_SHARED_LIBS OFF)
    set(PUGIXML_LINKAGE pugixml-static)
    set(CJSON_LINKAGE cjson)
else()
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs" FORCE)
    option(BUILD_SHARED_LIBS ON)
    set(PUGIXML_LINKAGE pugixml-shared)
    set(CJSON_LINKAGE cjson)
endif()
set(ENABLE_TARGET_EXPORT OFF CACHE BOOL "Enable target exporting" FORCE)
add_subdirectory(cJSON)

install(TARGETS cjson EXPORT levelapi_workarounds)

set(ENABLE_TARGET_EXPORT OFF CACHE BOOL "Enable target exporting" FORCE)
add_subdirectory(pugixml_patch)

install(TARGETS ${PUGIXML_LINKAGE} EXPORT levelapi_workarounds)

add_library(gmd2pp_hex ${COMPILE_MODE}
    ${gmd2pp_helper_hex}
)

add_library(base64 ${COMPILE_MODE} cpp-base64/base64.cpp)

install(TARGETS base64 EXPORT levelapi_workarounds)
install(TARGETS gmd2pp_hex EXPORT levelapi_workarounds)

add_library(gmd2 ${COMPILE_MODE} impl.cpp GDLevelSaveable.cpp)
target_link_libraries(gmd2 ext_ziplib gmdtools ${CJSON_LINKAGE} ${PUGIXML_LINKAGE} base64 gmd2pp_hex)