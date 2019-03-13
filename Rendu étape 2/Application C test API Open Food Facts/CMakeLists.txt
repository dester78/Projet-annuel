cmake_minimum_required(VERSION 3.13)
project(CScan)

set(CMAKE_C_STANDARD 99)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
include_directories(include)
include_directories(include/curl-7.64.0-win32-mingw)
link_directories(lib/curl-7.64.0-win32-mingw)

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


add_executable(
        CScan
        ${source_files}
        )

target_link_libraries(
        CScan
        curl
)

target_compile_options(CScan PRIVATE -Wall -Wextra -pedantic -Werror)
