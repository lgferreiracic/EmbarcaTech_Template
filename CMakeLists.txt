# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
pico_sdk_init()

set(FREERTOS_KERNEL_PATH "C:/FreeRTOS-Kernel")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(Template C CXX ASM)
add_subdirectory(lib/FatFs_SPI)    
add_subdirectory(src)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        PICO_PRINTF_SUPPORTS_FLOAT=1
        PICO_STDIO_ENABLE_PRINTF=1
)
target_link_libraries(${PROJECT_NAME} 
            pico_stdlib
            pico_time
            pico_cyw43_arch_lwip_threadsafe_background
            pico_lwip_mqtt
            pico_mbedtls
            pico_lwip_mbedtls

            hardware_i2c
            hardware_adc
            hardware_pwm
            hardware_timer
            hardware_gpio
            hardware_pio
            hardware_clocks
            hardware_uart
            hardware_spi
            hardware_rtc

            FatFs_SPI
            FreeRTOS-Kernel       
            FreeRTOS-Kernel-Heap4
)
target_include_directories(${PROJECT_NAME}  PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include/core
        ${CMAKE_CURRENT_LIST_DIR}/include/drivers
        ${CMAKE_CURRENT_LIST_DIR}/include/display
        ${CMAKE_CURRENT_LIST_DIR}/include/network
        ${CMAKE_CURRENT_LIST_DIR}/include/sensors
        ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS
        ${CMAKE_CURRENT_LIST_DIR}/lib/LWIP
        ${PICO_SDK_PATH}/lib/lwip/src/include
)
set(WEB_SRCS
    ${PICO_SDK_PATH}/lib/lwip/src/apps/http/httpd.c
    ${PICO_SDK_PATH}/lib/lwip/src/apps/http/fs.c
)
target_sources(${PROJECT_NAME}  PRIVATE ${WEB_SRCS})

set_source_files_properties(${WEB_SRCS} PROPERTIES
    COMPILE_FLAGS "-I${PICO_SDK_PATH}/lib/lwip/src/include/arch -I${PICO_SDK_PATH}/lib/lwip/src/include/lwip"
)

pico_enable_stdio_uart(${PROJECT_NAME}  0)
pico_enable_stdio_usb(${PROJECT_NAME}  1)


