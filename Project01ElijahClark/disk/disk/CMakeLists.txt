# //////////////////////////////////////////////////////////////////////////
# ///
# /// Copyright (c) 2020 Prof. AJ Bieszczad. All rights reserved.
# ///
# //////////////////////////////////////////////////////////////////////////

cmake_minimum_required(VERSION 3.13)

project(diskSimulator VERSION 1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall --pedantic -g -O0")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_BUILD_TYPE Debug)

add_executable(disk disk.c test_disk.c)
set_target_properties(disk PROPERTIES COMPILE_DEFINITIONS "__DEBUG_DISK")

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(LINUX)
    add_executable(disk_timer disk.c test_disk.c)
    set_target_properties(disk_timer PROPERTIES COMPILE_DEFINITIONS "__DEBUG_DISK;__DEBUG_TIMER")
    target_link_libraries(disk_timer rt)
else()
    message(">>> CANNOT BUILD disk_timer: Linux is required!")
endif()
