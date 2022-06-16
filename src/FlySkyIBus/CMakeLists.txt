#add_executable(FlySkyIBusDemo FlySkyIBusDemo.cpp FlySkyIBus.cpp FlySkyIBus.h)
add_executable(uart_test
        uart_test.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(uart_test pico_stdlib hardware_uart)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_test)
