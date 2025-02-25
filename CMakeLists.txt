cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(led-matrix)

set(BUILD_SHARED_LIBS OFF)

add_definitions(-DSTM32F401xE -DUSE_FULL_LL_DRIVER)

add_compile_options(-mcpu=cortex-m4 -mthumb -nostdlib -Wall -Wextra -Wpedantic)
add_link_options(-mcpu=cortex-m4 -mthumb -nostdlib)
add_link_options("-T${PROJECT_SOURCE_DIR}/src/linker_script.ld")

add_subdirectory(src)
