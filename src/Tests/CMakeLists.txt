add_executable(HelloBlink
        HelloBlink.c
        )

# pull in common dependencies
target_link_libraries(HelloBlink pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(HelloBlink)

# add url via pico_set_program_url
#example_auto_set_url(HelloBlink)
