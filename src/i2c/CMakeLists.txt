
add_library(i2c i2c.c)

target_link_libraries(i2c pico_stdlib hardware_i2c pico_binary_info)
