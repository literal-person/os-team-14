cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(temp_sensor C CXX ASM)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(temp_sensor main.c)

target_link_libraries(temp_sensor
    pico_stdlib
    hardware_adc
)

pico_enable_stdio_usb(temp_sensor 1)
pico_enable_stdio_uart(temp_sensor 0)

pico_add_extra_outputs(temp_sensor)
