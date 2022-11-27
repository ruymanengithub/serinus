#add_executable(mpu6050_i2c
#        mpu6050_i2c.c
#        )

add_executable(mpu6050Demo
        mpu6050Demo.c
        )
        
add_library(mpu6050
    mpu6050.c)
    

# pull in common dependencies
#target_link_libraries(mpu6050_i2c pico_stdlib hardware_i2c)

target_link_libraries(mpu6050Demo pico_stdlib hardware_i2c i2c mpu6050)

target_link_libraries(mpu6050 pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
#pico_add_extra_outputs(mpu6050_i2c)
pico_add_extra_outputs(mpu6050Demo)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
