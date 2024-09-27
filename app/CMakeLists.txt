cmake_minimum_required(VERSION 3.16)
project(pwm-uio VERSION 1.0.0 LANGUAGES C CXX)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} main.c)

add_subdirectory(ll)
add_subdirectory(uio)
add_subdirectory(pid)

target_link_libraries(${TARGET_NAME} PRIVATE ll uio pid)
target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)