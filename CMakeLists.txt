cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(led-matrix LANGUAGES C ASM)

set(BUILD_SHARED_LIBS OFF)

add_definitions(-DSTM32F401xE -DUSE_FULL_LL_DRIVER)


add_link_options("-T${PROJECT_SOURCE_DIR}/src/STM32F401RETX_FLASH.ld")

add_subdirectory(src)
