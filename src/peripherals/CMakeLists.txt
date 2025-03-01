add_library(max7219_led_matrix STATIC
    max7219_led_matrix.c
    sysclk.c
    ${PROJECT_SOURCE_DIR}/vendor/STM32F4_HAL_LL/Src/stm32f4xx_ll_gpio.c
    ${PROJECT_SOURCE_DIR}/vendor/STM32F4_HAL_LL/Src/stm32f4xx_ll_spi.c
    ${PROJECT_SOURCE_DIR}/vendor/STM32F4_HAL_LL/Src/stm32f4xx_ll_rcc.c
    ${PROJECT_SOURCE_DIR}/vendor/CMSIS_4/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c
)

target_include_directories(max7219_led_matrix PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/STM32F4_HAL_LL/Inc
    ${PROJECT_SOURCE_DIR}/vendor/CMSIS_4/CMSIS/Include
    ${PROJECT_SOURCE_DIR}/vendor/CMSIS_4/Device/ST/STM32F4/Include
)
