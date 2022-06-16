
add_subdirectory(FlySkyIBus)

add_subdirectory(Tests)

add_executable(picodrone
    picodrone.c
)

pico_enable_stdio_usb(picodrone 1)
pico_enable_stdio_uart(picodrone 1)

pico_add_extra_outputs(picodrone)

target_link_libraries(picodrone pico_stdlib)
