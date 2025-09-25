# Collect all driver sources for the main application
file(GLOB_RECURSE DRIVER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

# Make driver sources available to parent scope
set(DRIVER_SOURCES ${DRIVER_SOURCES} PARENT_SCOPE)

# Add all driver include directories
file(GLOB DRIVER_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/*/*)
foreach(dir ${DRIVER_DIRS})
    if(IS_DIRECTORY ${dir})
        include_directories(${dir})
    endif()
endforeach()