set(LIBRARY_NAME ll)
set(LIBRARY_SOURCES 
    capture.c
    clk.c
    pwm.c
    rw.c
    soc.c
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})
target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra)
target_include_directories(${LIBRARY_NAME} PUBLIC .)