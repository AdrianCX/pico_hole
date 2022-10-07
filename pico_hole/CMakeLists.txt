add_executable(pico_hole
  main.c
  dns_server.cpp
  check_allowed.cpp
  )

# enable usb output, disable uart output                                                                                                                                                                                                            
pico_enable_stdio_usb(pico_hole 1)
pico_enable_stdio_uart(pico_hole 0)

target_compile_definitions(pico_hole PRIVATE
  PICO_STDIO_DEFAULT_CRLF=1
  PICO_STDIO_ENABLE_CRLF_SUPPORT=1
  PICO_STACK_SIZE=0x4000 # 16K
  PICO_STDIO_STACK_BUFFER_SIZE=1024
  )
target_include_directories(pico_hole PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(pico_hole
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        pico_dns
        )

pico_add_extra_outputs(pico_hole)

target_include_directories(pico_hole PUBLIC config)
