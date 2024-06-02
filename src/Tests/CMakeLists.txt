add_executable(HelloBlink
        HelloBlink.c
        )

add_executable(hello_pwm
        hello_pwm.c
        )

add_executable(varSizes
        varSizes.c
        )

add_executable(Test_PWMvsFSIBus
        Test_PWMvsFSIBus.c
        )

add_executable(Test_PWMvsFSIBus_v2
        Test_PWMvsFSIBus_v2.c
        )
        
add_executable(Test_BMPandMPU
        Test_BMPandMPU.c
        )

add_executable(Test_FatFsSPI
        Test_FatFsSPI.c
        ../FatFs_SPI/hw_config.c
        )

# pull in common dependencies
target_link_libraries(HelloBlink pico_stdlib)
target_link_libraries(hello_pwm pico_stdlib hardware_pwm hardware_clocks)
target_link_libraries(varSizes pico_stdlib)
target_link_libraries(Test_PWMvsFSIBus pico_stdlib hardware_pwm hardware_uart hardware_clocks FlySkyIBus m)
target_link_libraries(Test_PWMvsFSIBus_v2 pico_stdlib hardware_pwm hardware_uart hardware_clocks FlySkyIBus m)
target_link_libraries(Test_BMPandMPU pico_stdlib bmp180 mpu6050 i2c m)
target_link_libraries(Test_FatFsSPI pico_stdlib FatFs_SPI)

# create map/bin/hex file etc.
pico_add_extra_outputs(HelloBlink)
pico_add_extra_outputs(hello_pwm)
pico_add_extra_outputs(varSizes)
pico_add_extra_outputs(Test_PWMvsFSIBus)
pico_add_extra_outputs(Test_PWMvsFSIBus_v2)
pico_add_extra_outputs(Test_BMPandMPU)
pico_add_extra_outputs(Test_FatFsSPI)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
