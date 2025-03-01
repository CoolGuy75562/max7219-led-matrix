add_subdirectory(peripherals)

set_property(SOURCE startup_stm32f401xe.s APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
add_executable(led_matrix.elf startup_stm32f401xe.s main.c)
target_include_directories(led_matrix.elf PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(led_matrix.elf max7219_led_matrix)
