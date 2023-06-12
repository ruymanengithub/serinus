
add_executable(ultrasoundDemo
        ultrasoundDemo.c
        )

add_library(ultrasound
    ultrasound.c)

# pull in common dependencies


target_link_libraries(ultrasoundDemo ultrasound)

target_link_libraries(ultrasound pico_stdlib hardware_gpio hardware_timer)

# create map/bin/hex file etc.

pico_add_extra_outputs(ultrasoundDemo)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
