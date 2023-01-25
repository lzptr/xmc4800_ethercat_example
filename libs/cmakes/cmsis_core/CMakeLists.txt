project(cmsis_core)


add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE 
                                ${CMAKE_CURRENT_LIST_DIR}/../../cmsis-core/
                                ${CMAKE_CURRENT_LIST_DIR}/../../cmsis-core/cmsis-core)

target_link_libraries(${PROJECT_NAME} INTERFACE bareMetal_xmc4800)