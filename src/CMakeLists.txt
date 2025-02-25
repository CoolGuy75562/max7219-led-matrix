add_subdirectory(peripherals)

add_executable(led_matrix.elf main.c startup.c)
target_include_directories(led_matrix.elf PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(led_matrix.elf max7219_led_matrix)
