cmake_minimum_required(VERSION 3.13)

# Set the arm toolchain path 
if ( NOT ARM_TOOLCHAIN_PATH )
    set( ARM_TOOLCHAIN_PATH /opt/toolchain/gcc-arm-none-eabi-10.3-2021.10/ )
endif()
message("Using following path for arm toolchain: ${ARM_TOOLCHAIN_PATH}")
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES ARM_TOOLCHAIN_PATH)

# Set toolchain file from the platform (Needs to be set before the project name)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/libs/platforms/toolchain/arm-none-eabi.cmake)

project(xmc4800_ethercat_example)

enable_language(C CXX ASM)

# Generate compile_commands.json for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add some binary output directories 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


##### Project Setup #####

set(XMC4800_LINKER_FILE ${CMAKE_CURRENT_LIST_DIR}/libs/mtb-xmclib-cat3/CMSIS/Infineon/COMPONENT_XMC4800/Source/TOOLCHAIN_GCC_ARM/XMC4800x2048.ld)
        
# Add additional modules
include(${CMAKE_CURRENT_LIST_DIR}/libs/platforms/toolchain/firmware.cmake)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/platforms/bareMetal_xmc4800)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/cmakes/cmsis_core)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/cmakes/xmc4800_lib)


# Add the subdirs with the actual code
add_subdirectory(src)