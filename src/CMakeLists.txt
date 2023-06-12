
cmake_minimum_required(VERSION 3.22.1)

#project(serinus)

add_subdirectory(FlySkyIBus)

add_subdirectory(Tests)

add_subdirectory(MPU6050)

add_subdirectory(BMP180)

add_subdirectory(bmp085Driv)

add_subdirectory(i2c)

add_subdirectory(PWM)

add_subdirectory(PID)

add_subdirectory(ultrasound)

add_executable(${PROJECT_NAME} picodrone.c)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_gpio pico_binary_info
    PID mpu6050 bmp180 i2c bmp085 ultrasound pwm flyskyibus)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})


