set(LIBRARY_NAME uio)
set(LIBRARY_SOURCES 
    uio_helper.c
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})
target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra)
target_include_directories(${LIBRARY_NAME} PUBLIC .)