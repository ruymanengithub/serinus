

add_executable(pwmDemo
        pwmDemo.c
        )

target_link_libraries(pwmDemo pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwmDemo)

