add_executable(bmp180Demo
        bmp180Demo.c
)
        
        
add_library(bmp180
    bmp180.c)
    

target_link_libraries(bmp180 pico_stdlib hardware_i2c)
target_link_libraries(bmp180Demo bmp180)

# create map/bin/hex file etc.
#pico_add_extra_outputs(bmp180)
pico_add_extra_outputs(bmp180Demo)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
