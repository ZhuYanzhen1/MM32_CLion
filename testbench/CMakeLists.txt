cmake_minimum_required(VERSION 3.20)
project(testbench C)

set(CMAKE_C_STANDARD 11)

if ("${CMAKE_BUILD_PLATFORM}" STREQUAL "laozhu")
    link_directories(D:/msys64/opt/cunit/lib)
    include_directories(D:/msys64/opt/cunit/include script)
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "laozhu")
    link_directories(C:/zgl/cunit/lib)
    include_directories(C:/zgl/cunit/include script)
elseif ("${CMAKE_BUILD_PLATFORM}" STREQUAL "github")
    link_directories(/home/runner/work/MM32_CLion/MM32_CLion/cunit/lib)
    include_directories(/home/runner/work/MM32_CLion/MM32_CLion/cunit/include script)
endif ()

file(GLOB_RECURSE SOURCES "main.c" "unit_test.c" "script/*.c")
add_executable(testbench ${SOURCES})

target_link_libraries(testbench cunit)
