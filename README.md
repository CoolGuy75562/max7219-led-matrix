Small driver for a MAX7219 8x8 LED matrix module for an STM32F401RE microcontroller using CMSIS and ST LL drivers. A simple example programme is in main.c.

Building requires CMake, the GNU ARM toolchain, OpenOCD, as well as CMSIS_4, cmsis-device-f4, and stm32f4xx-hal (see below) to use the ST LL driver.

```bash
mkdir vendor
cd vendor
git clone https://github.com/ARM-software/CMSIS_4.git
```

Now in the Devices directory, create a new directory ST, and clone in the cmsis-device-f4 module

```bash
cd Devices
mkdir ST
cd ST
git clone https://github.com/STMicroelectronics/cmsis-device-f4.git STM32F4
```

Return to the root of the vendor directory and clone the module containing the HAL and LL drivers

```bash
git clone https://github.com/STMicroelectronics/stm32f4xx-hal-driver.git STM32F4_HAL_LL
```

Now we are ready to build the project. Return to the project root directory and run cmake, then make

```bash
cmake .
make
```

The compiled binary led_matrix.elf should now be contained in the /src directory. To flash it onto the microcontroller, run

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program led_matrix.elf verify reset exit"
```
