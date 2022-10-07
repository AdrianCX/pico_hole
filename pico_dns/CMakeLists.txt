add_library(pico_dns INTERFACE)

target_sources(pico_dns INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/codec.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mappings.c
  )

target_include_directories(pico_dns INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
