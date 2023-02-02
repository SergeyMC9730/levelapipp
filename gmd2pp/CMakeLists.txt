cmake_minimum_required(VERSION 3.0.0)
project(gmd2 VERSION 1.0.0)

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

add_library(ext_bzip2 ${bzip2_files})
add_library(ext_lzma ${lzma_files})
add_library(ext_zlib ${zlib_files})

message(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)

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

add_library(ext_ziplib ${ziplib_files})
link_libraries(ext_ziplib ext_bzip2 ext_lzma ext_zlib)

add_library(gmd2 impl.cpp)
target_link_libraries(gmd2 ext_ziplib)

