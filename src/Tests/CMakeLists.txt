add_executable(HelloBlink
        HelloBlink.c
        )

add_executable(hello_pwm,
        hello_pwhm.c
        )       

# pull in common dependencies
target_link_libraries(HelloBlink pico_stdlib)

target_link_libraries(hello_pwm pico_stdlib, hardware_pwm, hardware_clocks)


# create map/bin/hex file etc.
pico_add_extra_outputs(HelloBlink)
pico_add_extra_outputs(hello_pwm)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
