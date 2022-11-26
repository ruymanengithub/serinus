
        
add_library(bmp180
    bmp180.c)
    

target_link_libraries(bmp180 pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
#pico_add_extra_outputs(mpu6050_i2c)
#pico_add_extra_outputs(mpu6050Demo)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
