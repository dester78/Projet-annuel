cmake_minimum_required(VERSION 3.13)
project(CScan)

set(CMAKE_C_STANDARD 99)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
include_directories(include)
include_directories(include/curl-7.64.0-win32-mingw)
link_directories(lib/curl-7.64.0-win32-mingw )

file(
        GLOB_RECURSE
        source_files
        src/main.c
        src/curlHandler.c
        src/fileHandler.c
        src/jsonHandler.c
        src/jsonReader.c
        src/jsonWriter.c
        src/wcharHandler.c

)

FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(CURL REQUIRED libcurl)


INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS}  ${CURL_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER}  ${CURL_CFLAGS_DIRS})


add_executable(
        CScan
        ${source_files}
        )

target_link_libraries(
        CScan
        curl
)

TARGET_LINK_LIBRARIES(CScan ${GTK3_LIBRARIES})

target_compile_options(CScan PRIVATE -Wall -Wextra -pedantic)
