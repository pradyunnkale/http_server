add_library(cmsis_5 INTERFACE)
target_include_directories(cmsis_5 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
)

add_library(cmsis_device_h7 INTERFACE)
target_include_directories(cmsis_device_h7 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis-device-h7/Include
)

file(GLOB HAL_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h7xx_hal_driver/Src/*.c
)

add_library(stm32h7xx_hal_driver STATIC ${HAL_SOURCES})
target_include_directories(stm32h7xx_hal_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h7xx_hal_driver/Inc
    ${CMAKE_SOURCE_DIR}/config  # Add config directory for HAL conf files
)

target_link_libraries(stm32h7xx_hal_driver PUBLIC 
    cmsis_5           # Provides core_cm7.h
    cmsis_device_h7   # Provides stm32h753xx.h
)

add_library(lib INTERFACE)
target_link_libraries(lib INTERFACE
    cmsis_5
    cmsis_device_h7  
    stm32h7xx_hal_driver
)