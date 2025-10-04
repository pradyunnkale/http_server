
add_executable(${PROJECT_NAME}
    server/main.c
    ${CMAKE_SOURCE_DIR}/drivers/ethernet/ethernet.c
    ${CMAKE_SOURCE_DIR}/drivers/ethernet/ethernetif.c
    ${CMAKE_SOURCE_DIR}/startup/startup_stm32h753xx.s
    ${CMAKE_SOURCE_DIR}/config/system_stm32h7xx.c
    ${CMAKE_SOURCE_DIR}/config/stm32h7xx_it.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32H753ZITx_FLASH.ld"
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/lib/stm32h7xx_hal_driver/Inc
    ${CMAKE_SOURCE_DIR}/lib/cmsis-device-h7/Include
    ${CMAKE_SOURCE_DIR}/lib/CMSIS_5/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/lib/FreeRTOS/include
    ${CMAKE_SOURCE_DIR}/lib/FreeRTOS/portable/GCC/ARM_CM7/r0p1
    ${CMAKE_SOURCE_DIR}/drivers/ethernet
)
