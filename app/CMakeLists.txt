add_executable(${PROJECT_NAME}
    display/main.cpp
    # Add your driver sources directly here when you create them:
    ../drivers/ledcontrol/ledcontrol.cpp
    # ../drivers/motor/stepper.c
    # ../drivers/led/led_matrix.c
    # ../drivers/sensors/hall_sensor.c
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/startup/startup_stm32h753xx.s
    ${CMAKE_SOURCE_DIR}/config/system_stm32h7xx.c
    ${CMAKE_SOURCE_DIR}/config/stm32h7xx_it.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32H753ZITx_FLASH.ld"
)

target_link_libraries(${PROJECT_NAME} lib)