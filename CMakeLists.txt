cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/arm-none-eabi-gcc.cmake)

project(http_server LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

add_compile_definitions(USE_HAL_DRIVER STM32H753xx) 

# Include Subirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/drivers)
add_subdirectory(${CMAKE_SOURCE_DIR}/app)

include_directories(
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/lib/cmsis-device-h7/Include
    ${CMAKE_SOURCE_DIR}/lib/stm32h7xx_hal_driver/Inc
    ${CMAKE_SOURCE_DIR}/lib/CMSIS_5/CMSIS/Core/Include
)

if (TARGET ${PROJECT_NAME})
    add_custom_target(flash
        COMMAND openocd -f ${CMAKE_SOURCE_DIR}/tools/openocd.cfg -c "program $<TARGET_FILE:${PROJECT_NAME}> verify reset exit"
        DEPENDS ${PROJECT_NAME}
        COMMENT "Flashing ${PROJECT_NAME} to target"
    )
endif()
