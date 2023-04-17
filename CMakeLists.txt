cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_pilot_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_executable(pico_pilot)
target_sources(pico_pilot PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        ${CMAKE_CURRENT_LIST_DIR}/irdecoders/necdecoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/irdecoders/basedecoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/irdecoders/rc5decoder.cpp
        )
# Make sure TinyUSB can find tusb_config.h
target_include_directories(pico_pilot PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pico_pilot PUBLIC pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(pico_pilot)

add_executable(pico_test_USB)
target_sources(pico_test_USB PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/test_USB_main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )
# Make sure TinyUSB can find tusb_config.h
target_include_directories(pico_test_USB PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pico_test_USB PUBLIC pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(pico_test_USB)


add_executable(pico_test_pilot)
target_sources(pico_test_pilot PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/test_pilot_main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/irdecoders/necdecoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/irdecoders/basedecoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/irdecoders/rc5decoder.cpp
        )
target_link_libraries(pico_test_pilot PUBLIC pico_stdlib)
pico_enable_stdio_usb(pico_test_pilot 1)
pico_enable_stdio_uart(pico_test_pilot 1)
pico_add_extra_outputs(pico_test_pilot)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(dev_hid_composite PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)


