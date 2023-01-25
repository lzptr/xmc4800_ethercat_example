# Create the executable target
add_executable(${CMAKE_PROJECT_NAME}
                    main.c
)

# Link the libraries
target_link_libraries (${CMAKE_PROJECT_NAME} 
                        PUBLIC 
                            gcc
                            c 
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
                            --specs=nano.specs
                            -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.map
)

# Add the binary for flashing
create_hex_output(${CMAKE_PROJECT_NAME})

# Show the section sizes of the elf file
print_sizes(${CMAKE_PROJECT_NAME})
