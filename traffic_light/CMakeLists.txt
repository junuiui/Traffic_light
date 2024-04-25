cmake_minimum_required(VERSION 3.18)
project(traffic_light VERSION 1.0 DESCRIPTION "Traffic Light: Buzzer + PRU + NeoPixel" LANGUAGES C)

set(CMAKE_C_STANDARD 11)
# add_compile_options(-Wall -Wpedantic -Wextra)
# add_compile_options(-Werror -Wpedantic -Wextra)

add_subdirectory(app)
add_subdirectory(hal)
add_subdirectory(pru-tl)