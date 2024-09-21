set(LIBRARY_NAME ll)
set(LIBRARY_SOURCES 
    src/capture.c
    src/clk.c
    src/pwm.c
    src/rw.c
    src/soc.c
    src/config.c
)

add_library(
    ${LIBRARY_NAME} STATIC 
    ${LIBRARY_SOURCES}
)

target_compile_options(
    ${LIBRARY_NAME} PRIVATE 
    -Wall 
    -Wextra
    -Werror=return-type
)

target_include_directories(${LIBRARY_NAME} PUBLIC inc)