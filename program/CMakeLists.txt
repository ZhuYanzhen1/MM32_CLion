#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.15)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(mm32f3277 C CXX ASM)
set(PROCESS_MCU Process_MM32)
set(CONTROL_MCU Control_MM32)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_options(-mfloat-abi=soft)
add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

message(STATUS "Optimization for speed")
#add_compile_options(-O0)
add_compile_options(-O2)

#add_definitions(-DUSE_HAL_DRIVER)

add_custom_target(version_h BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/../system/version.h"
        COMMAND ${CMAKE_COMMAND}
        -D SRCDIR="${CMAKE_CURRENT_SOURCE_DIR}"
        -P ${CMAKE_CURRENT_SOURCE_DIR}/gitversion.cmake)

include_directories(library/regs system system/cmsis library/include proc_mcu hardware system/official
        math device debug protocol gui ctrl_mcu freertos form form/origin_form form/fusion_form form/calibrate_form
        form/debug_form fatfs)

file(GLOB_RECURSE CONTROL_SOURCES "system/official/startup_mm32f327.s" "library/source/*.c" "math/*.c"
        "system/*.c" "system/official/*.c" "math/qfplib.s" "debug/*.c" "protocol/*.c" "debug/cmb_fault.S"
        "hardware/*.c" "ctrl_mcu/*.c" "device/sensor_decode.c" "device/tfcard.c" "fatfs/*.c")

file(GLOB_RECURSE PROCESS_SOURCES "system/official/startup_mm32f327.s" "library/source/*.c" "math/*.c"
        "proc_mcu/*.c" "hardware/*.c" "system/*.c" "system/official/*.c" "math/qfplib.s" "device/*.c"
        "debug/*.c" "protocol/*.c" "debug/cmb_fault.S" "gui/*.c" "freertos/*.c" "form/*.c")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/mm32f3277.ld)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROCESS_MCU}.elf ${PROCESS_SOURCES} ${LINKER_SCRIPT})
add_dependencies(${PROCESS_MCU}.elf version_h)
set(PROCESS_HEX_FILE ${PROJECT_BINARY_DIR}/${PROCESS_MCU}.hex)
add_custom_command(TARGET ${PROCESS_MCU}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROCESS_MCU}.elf> ${PROCESS_HEX_FILE}
        COMMENT "Building ${HEX_FILE}")

add_executable(${CONTROL_MCU}.elf ${CONTROL_SOURCES} ${LINKER_SCRIPT})
add_dependencies(${CONTROL_MCU}.elf version_h)
set(CONTROL_HEX_FILE ${PROJECT_BINARY_DIR}/${CONTROL_MCU}.hex)
add_custom_command(TARGET ${CONTROL_MCU}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CONTROL_MCU}.elf> ${CONTROL_HEX_FILE}
        COMMENT "Building ${HEX_FILE}")

set_target_properties(${PROCESS_MCU}.elf PROPERTIES COMPILE_DEFINITIONS "IS_PROCESS_MCU")
set_target_properties(${CONTROL_MCU}.elf PROPERTIES COMPILE_DEFINITIONS "IS_CONTROL_MCU")
