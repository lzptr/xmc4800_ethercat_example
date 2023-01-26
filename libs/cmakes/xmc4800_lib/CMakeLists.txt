project(xmc4800_lib)

file(GLOB_RECURSE PROJ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../mtb-xmclib-cat3/XMCLib/src/*.c")


add_library(${PROJECT_NAME} STATIC 
            ${PROJ_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/../../mtb-xmclib-cat3/CMSIS/Infineon/COMPONENT_XMC4800/Source/system_XMC4800.c
            ${CMAKE_CURRENT_LIST_DIR}/../../mtb-xmclib-cat3/CMSIS/Infineon/COMPONENT_XMC4800/Source/TOOLCHAIN_GCC_ARM/startup_XMC4800.S
)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../../mtb-xmclib-cat3/XMCLib/inc
  ${CMAKE_CURRENT_LIST_DIR}/../../mtb-xmclib-cat3/CMSIS/Infineon/COMPONENT_XMC4800/Include
)

target_link_libraries(${PROJECT_NAME} PUBLIC bareMetal_xmc4800 cmsis_core)