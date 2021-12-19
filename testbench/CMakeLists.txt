cmake_minimum_required(VERSION 3.21)
project(testbench C)

set(CMAKE_C_STANDARD 11)

set(COMMON_INCLUDE "script" "../program/debug")

if ("${CMAKE_BUILD_PLATFORM}" STREQUAL "laozhu")
    link_directories(D:/msys64/opt/cunit/lib)
    include_directories(D:/msys64/opt/cunit/include ${COMMON_INCLUDE})
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "zgl")
    link_directories(C:/zlg/msys64/opt/cunit/lib)
    include_directories(C:/zlg/msys64/opt/cunit/include ${COMMON_INCLUDE})
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "github")
    link_directories(/home/runner/work/MM32_CLion/MM32_CLion/cunit/lib)
    include_directories(/home/runner/work/MM32_CLion/MM32_CLion/cunit/include ${COMMON_INCLUDE})
endif ()

add_definitions(-DRUNNING_UNIT_TEST)

file(GLOB_RECURSE SOURCES "main.c" "unit_test.c" "script/*.c" "../program/debug/encrypt.c")
add_executable(testbench ${SOURCES})

target_link_libraries(testbench cunit)
