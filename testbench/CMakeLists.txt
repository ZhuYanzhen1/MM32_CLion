cmake_minimum_required(VERSION 3.18)
project(testbench C)
set(CMAKE_C_STANDARD 11)

add_compile_options(-Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -O0)
add_link_options(-Wno-pointer-to-int-cast -Wno-int-to-pointer-cast)
add_definitions(-DRUNNING_UNIT_TEST)
if ("${CMAKE_BUILD_PLATFORM}" STREQUAL "github")
    add_definitions(-DCMAKE_GITHUB_BUILD)
endif ()

set(COMMON_INCLUDE "script" "../program/protocol" "../program/math")

if ("${CMAKE_BUILD_PLATFORM}" STREQUAL "laozhu")
    link_directories(D:/msys64/opt/cunit/lib)
    include_directories(D:/msys64/opt/cunit/include ${COMMON_INCLUDE})
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "zgl")
    link_directories(C:/zlg/msys64/opt/cunit/lib)
    include_directories(C:/zlg/msys64/opt/cunit/include ${COMMON_INCLUDE})
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "github")
    link_directories(/home/runner/work/MM32_CLion/MM32_CLion/cunit/lib)
    include_directories(/home/runner/work/MM32_CLion/MM32_CLion/cunit/include ${COMMON_INCLUDE})
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "codeserver")
    link_directories(/opt/cunit/lib)
    include_directories(/opt/cunit/include ${COMMON_INCLUDE})
endif ()

file(GLOB_RECURSE SOURCES "main.c" "unit_test.c" "script/*.c" "../program/protocol/mdtp_pack.c"
        "../program/protocol/gps_parser.c" "../program/protocol/mdtp_unpack.c" "../program/math/sort.c"
        "../program/math/riccati_solver.c" "../program/math/mag_calibrate.c" "../program/math/fast_math.c")

add_executable(testbench ${SOURCES})
target_link_libraries(testbench cunit m)
