
function(pico_https_init)
  set(PICO_BOARD "pico_w")
  set(ENV{'PICO_BOARD'} "pico_w")
  set(PICO_SDK_PATH ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/pico-sdk)
  set(PICO_MBEDTLS_PATH ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mbedtls)

  set(FAMILY rp2040)
  set(BOARD pico_sdk)
  #set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

           
  message("${CMAKE_CURRENT_LIST_DIR}")
  # Pull in SDK (must be before project)
  include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/pico_sdk_import.cmake)
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

  add_subdirectory(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ pico_mbedtls)
  add_subdirectory(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ pico_tls)
endfunction()