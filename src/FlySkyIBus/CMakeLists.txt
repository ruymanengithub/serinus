#add_executable(FlySkyIBusDemo FlySkyIBusDemo.cpp FlySkyIBus.cpp FlySkyIBus.h)
#add_executable(uart_test uart_test.c)

add_executable(FlySkyIBusDemo FlySkyIBusDemo.c)
#add_executable(FlySkyIBus FlySkyIBus.c)
add_library(FlySkyIBus FlySkyIBus.c)

# pull in common dependencies and additional uart hardware support
#target_link_libraries(uart_test pico_stdlib hardware_uart)
target_link_libraries(FlySkyIBus pico_stdlib hardware_uart hardware_irq hardware_timer)
target_link_libraries(FlySkyIBusDemo FlySkyIBus)


# create map/bin/hex file etc.
#pico_add_extra_outputs(uart_test)
pico_add_extra_outputs(FlySkyIBusDemo)
