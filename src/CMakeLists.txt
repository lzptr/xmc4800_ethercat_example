# Create the executable target
file(GLOB_RECURSE PROJ_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

add_custom_command (
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sii_eeprom.o
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sii_eeprom.bin
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${CMAKE_OBJCOPY} -I binary -O elf32-littlearm -B arm sii_eeprom.bin ${CMAKE_CURRENT_BINARY_DIR}/sii_eeprom.o
  )

add_executable(${CMAKE_PROJECT_NAME}
                    ${PROJ_SOURCES}
                    sii_eeprom.o
)

# Add project ethercat options to soem lib...
target_include_directories(soes PUBLIC ${CMAKE_CURRENT_LIST_DIR})


# Link the libraries
target_link_libraries (${CMAKE_PROJECT_NAME} 
                        PUBLIC 
                            gcc
                            c 
                            soes
                            xmc4800_lib
)


# Add some more project specific compile commands if neccessary
# Can be also added through cmake
target_compile_options(${CMAKE_PROJECT_NAME} 
                        PUBLIC
)

# Link with the nano C std library
target_link_options(${CMAKE_PROJECT_NAME} 
                            PUBLIC
                            --specs=nosys.specs
)

# Add the binary for flashing
create_hex_output(${CMAKE_PROJECT_NAME})

# Show the section sizes of the elf file
print_sizes(${CMAKE_PROJECT_NAME})
