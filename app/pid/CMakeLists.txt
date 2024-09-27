set(LIBRARY_NAME pid)
set(LIBRARY_SOURCES 
    pid.c
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})
target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra)
target_include_directories(${LIBRARY_NAME} PUBLIC .)