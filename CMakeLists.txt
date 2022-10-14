cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD "pico_w")
set(ENV{'PICO_BOARD'} "pico_w")
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk)
set(PICO_MBEDTLS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls)

set(FAMILY rp2040)
set(BOARD pico_sdk)
#set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
project(pico_playground C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-O2 -march=armv6-m -mcpu=cortex-m0plus
  -Wall
  -Wno-unused-result
  -Wno-format
  -Wno-unused-function
  -Wno-maybe-uninitialized
  -Werror
  )

add_subdirectory(pico_dns)
add_subdirectory(pico_hole)
add_subdirectory(random_leds)
