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

#include "peripherals/sysclk.h"
#include "peripherals/max7219_led_matrix.h"

#define BITMAP_LEVEL_0 (uint64_t)(0x00)
#define BITMAP_LEVEL_1 (uint64_t)(0x0000001818000000)
#define BITMAP_LEVEL_2 (uint64_t)(0x00003C3C3C3C0000)
#define BITMAP_LEVEL_3 (uint64_t)(0x007E7E7E7E7E7E00)
#define BITMAP_LEVEL_4 (uint64_t)(0xFFFFFFFFFFFFFFFF)

#define WAIT() for (volatile int i = 0; i < 1000000; i++)

void main(void) {

  sysclk_init();
  max7219_lm_init();

  void (*led_fun)(uint8_t, uint8_t) = &max7219_lm_set_led;
  
  for (;;) {
    for (uint8_t y = 0; y < 8; y++) {
      if (y & 1) {
        for (uint8_t x = 0; x < 8; x++) {
          WAIT();
	  led_fun(x, y);
        }
      } else {
        for (uint8_t x = 0; x < 8; x++) {
	  WAIT();
	  led_fun(7 - x, y);
        }
      }
    }
    if (led_fun == &max7219_lm_set_led) {
      led_fun = &max7219_lm_unset_led;
    } else {
      led_fun = &max7219_lm_set_led;
    }
  }
}
