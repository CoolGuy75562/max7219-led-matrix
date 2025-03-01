set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_compile_options(-g -mcpu=cortex-m4 -mthumb --specs=nosys.specs -Wall -Wextra -Wpedantic)
add_link_options(-mcpu=cortex-m4 -mthumb --specs=nosys.specs)