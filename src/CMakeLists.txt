file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    main.c
    ${CMAKE_CURRENT_LIST_DIR}/*.c
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../include
)
pico_add_extra_outputs(${PROJECT_NAME})

set(PIO_FILE ${CMAKE_CURRENT_LIST_DIR}/drivers/ws2812.pio)
pico_generate_pio_header(${PROJECT_NAME} ${PIO_FILE})
