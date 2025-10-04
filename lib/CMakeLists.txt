
# lib/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(libs C CXX ASM)

# ------------------------------
# CMSIS Core
# ------------------------------
add_library(cmsis_core INTERFACE)
target_include_directories(cmsis_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
)

# CMSIS Device H7
add_library(cmsis_device INTERFACE)
target_include_directories(cmsis_device INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis-device-h7/Include
)

# ------------------------------
# HAL Driver
# ------------------------------
file(GLOB HAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h7xx_hal_driver/Src/*.c
)

add_library(hal OBJECT ${HAL_SOURCES})
target_include_directories(hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h7xx_hal_driver/Inc
    ${CMAKE_SOURCE_DIR}/config
)
target_link_libraries(hal PUBLIC cmsis_core cmsis_device)

# ------------------------------
# FreeRTOS
# ------------------------------
file(GLOB FREERTOS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/portable/GCC/ARM_CM7/r0p1/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/portable/MemMang/heap_4.c
)

add_library(freertos OBJECT ${FREERTOS_SOURCES})
target_include_directories(freertos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/portable/GCC/ARM_CM7/r0p1
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h7xx_hal_driver/Inc
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis-device-h7/Include
)
target_link_libraries(freertos PUBLIC hal)

#-------------------------------
# LWIP
#-------------------------------
file(GLOB LWIP_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/core/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/core/ipv4/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/netif/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/api/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/contrib/ports/freertos/sys_arch.c
)

add_library(lwip OBJECT ${LWIP_SRC})
target_include_directories(lwip PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/include/ipv4
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/include/ipv6
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/contrib/ports/freertos/include
    ${CMAKE_SOURCE_DIR}/config
)

target_link_libraries(lwip PUBLIC freertos hal cmsis_core cmsis_device)
# ------------------------------
# Top-level static library combining HAL + FreeRTOS as OBJECTS
# ------------------------------
add_library(lib STATIC
    $<TARGET_OBJECTS:hal>
    $<TARGET_OBJECTS:freertos>
    $<TARGET_OBJECTS:lwip>
)
target_link_libraries(lib PUBLIC cmsis_core cmsis_device lwip)
