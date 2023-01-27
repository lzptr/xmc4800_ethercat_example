# (Modern) CMake for the soes project 

project(soes)

file(GLOB PROJ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../SOES/soes/*.c")


add_library(${PROJECT_NAME} STATIC 
            ${PROJ_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SOES/soes
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SOES/soes/include/sys/gcc
)

target_link_libraries(${PROJECT_NAME} PUBLIC xmc4800_lib)