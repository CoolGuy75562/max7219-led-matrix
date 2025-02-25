/**
 *  Copyright (C) 2025  Angus McLean
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#ifndef MAX7219_LED_MATRIX_H
#define MAX7219_LED_MATRIX_H

#include <stdint.h>

/**
 * Pins used:
 *            PA5 -> SCK
 *            PA6 -> CS
 *            PA7 -> DIN
 **/

/**
 * Configures and initialises MAX7219 chip and
 * above pins for SPI and GPIO, and blanks the LED matrix.
 **/
void max7219_lm_init(void);

/**
 * Set the LED in position (x, y), 0 <= x, y < 8
 **/
void max7219_lm_set_led(uint8_t x, uint8_t y);

/**
 * Unset the LED in position (x, y), 0 <= x, y < 8
 **/
void max7219_lm_unset_led(uint8_t x, uint8_t y);

/**
 * Set LED matrix according to bitmap, 0 = off, 1 = on,
 * MSB = top left, LSB = bottom right
 **/
void max7219_lm_set_matrix(uint64_t bitmap);

#endif
