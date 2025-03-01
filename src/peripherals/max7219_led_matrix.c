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

#include "peripherals/max7219_led_matrix.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_spi.h"

#define MAX7219_CS_PORT (GPIOA)
#define MAX7219_CS_PIN (LL_GPIO_PIN_6)

#define MAX7219_SPI_PORT (GPIOA)
#define MAX7219_SPI_CHANNEL (SPI1)

#define MAX7219_SCK_PORT (MAX7219_SPI_PORT)
#define MAX7219_SCK_PIN (LL_GPIO_PIN_5)

#define MAX7219_MOSI_PORT (MAX7219_SPI_PORT)
#define MAX7219_MOSI_PIN (LL_GPIO_PIN_7)

#define LM_ROWS (uint8_t)(8)
#define LM_COLS (uint8_t)(8)

#define SHIFT_PACKET_ADDR (uint8_t)(8)
#define MASK_PACKET_DATA (uint8_t)(0xFF)

enum {
  MAX7219_NOP = 0x0,
  MAX7219_D0 = 0x1,
  MAX7219_D1 = 0x2,
  MAX7219_D2 = 0x3,
  MAX7219_D3 = 0x4,
  MAX7219_D4 = 0x5,
  MAX7219_D5 = 0x6,
  MAX7219_D6 = 0x7,
  MAX7219_D7 = 0x8,
  MAX7219_DEC_MODE = 0x9,
  MAX7219_INTENSITY = 0xA,
  MAX7219_SCAN_LIM = 0xB,
  MAX7219_SHUTDOWN = 0xC,
  MAX7219_DISP_TEST = 0xF
};

#define PACKET_SHUTDOWN_ON (uint16_t)(MAX7219_SHUTDOWN << SHIFT_PACKET_ADDR)
#define PACKET_SHUTDOWN_OFF (uint16_t)((MAX7219_SHUTDOWN << SHIFT_PACKET_ADDR) | 1)
#define PACKET_DECODE_OFF (uint16_t)(MAX7219_DEC_MODE << SHIFT_PACKET_ADDR)
#define PACKET_SCAN_LIM_ALL (uint16_t)((MAX7219_SCAN_LIM << SHIFT_PACKET_ADDR) | 7)
#define PACKET_MIN_INTENSITY (uint16_t)(MAX7219_INTENSITY << SHIFT_PACKET_ADDR)

#define CHIP_SELECT(MAX7219_CS)                                                \
  do {                                                                         \
    LL_GPIO_ResetOutputPin(MAX7219_CS_PORT, MAX7219_CS_PIN);                   \
  } while (0)

#define CHIP_UNSELECT(MAX7219_CS)                                              \
  do {                                                                         \
    while (LL_SPI_IsActiveFlag_BSY(MAX7219_SPI_CHANNEL))                       \
      ;                                                                        \
    LL_GPIO_SetOutputPin(MAX7219_CS_PORT, MAX7219_CS_PIN);                     \
  } while (0)

#ifdef DEBUG

#define MAX7219_ASSERT(statement)                                              \
  do {                                                                         \
    if (!(statement)) {                                                        \
      asm("bkpt");                                                             \
    }                                                                          \
  } while (0)

#else

#define MAX7219_ASSERT(statement)

#endif

static void update_matrix(void);
static void update_matrix_row(uint8_t y);
static void send_packet(uint16_t packet);

static volatile uint64_t led_matrix = 0;

void max7219_lm_init(void) {

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /* D13/PA5 -> MAX7219_SCK
   * D11/PA7 -> MAX7219_MOSI */
  LL_GPIO_InitTypeDef GPIO_init_struct = {
      .Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7,
      .Mode = LL_GPIO_MODE_ALTERNATE,
      .Speed = LL_GPIO_SPEED_FREQ_HIGH, 
      .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
      .Pull = LL_GPIO_PULL_NO,
      .Alternate = LL_GPIO_AF_5 /* SPI1..4 */
  };
  LL_GPIO_Init(MAX7219_SPI_PORT, &GPIO_init_struct);

  LL_SPI_InitTypeDef SPI_init_struct = {
      .TransferDirection = LL_SPI_HALF_DUPLEX_TX,
      .Mode = LL_SPI_MODE_MASTER,
      .DataWidth = LL_SPI_DATAWIDTH_16BIT,
      .ClockPolarity = LL_SPI_POLARITY_HIGH,
      .ClockPhase = LL_SPI_PHASE_1EDGE,
      .NSS = LL_SPI_NSS_SOFT,
      .BaudRate =
      LL_SPI_BAUDRATEPRESCALER_DIV16,
      .BitOrder = LL_SPI_MSB_FIRST,
      .CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE,
      .CRCPoly = 0x00};
  LL_SPI_Init(MAX7219_SPI_CHANNEL, &SPI_init_struct);

  /* D12/PA6 -> MAX7219_CS */
  GPIO_init_struct.Pin = LL_GPIO_PIN_6;
  GPIO_init_struct.Mode = LL_GPIO_MODE_OUTPUT;
  LL_GPIO_Init(GPIOA, &GPIO_init_struct);

  /* CS high */
  CHIP_UNSELECT(MAX7219_CS);

  LL_SPI_Enable(MAX7219_SPI_CHANNEL);

  /* Enable all digits */
  send_packet(PACKET_SCAN_LIM_ALL);
  
  /* Turn off decoding for all digits */
  send_packet(PACKET_DECODE_OFF);

  /* Set intensity to minimum */
  send_packet(PACKET_MIN_INTENSITY);
  
  /* Turn shutdown mode off */
  send_packet(PACKET_SHUTDOWN_OFF);
  
  /* Blank all LEDs */
  update_matrix();
}

void max7219_lm_set_led(uint8_t x, uint8_t y) {
  MAX7219_ASSERT((x < LM_COLS) && (y < LM_ROWS));
  led_matrix |= ((uint64_t)1 << (LM_COLS * y + x));
  update_matrix_row(y);
}

void max7219_lm_unset_led(uint8_t x, uint8_t y) {
  MAX7219_ASSERT((x < LM_COLS) && (y < LM_ROWS));
  led_matrix &= ~((uint64_t)1 << (LM_COLS * y + x));
  update_matrix_row(y);
}

void max7219_lm_set_matrix(uint64_t bitmap) {
  led_matrix = bitmap;
  update_matrix();
}

static void update_matrix(void) {
  for (uint8_t y = 0; y < LM_ROWS; y++) {
    update_matrix_row(y);
  }
}

static void update_matrix_row(uint8_t y) {
  uint16_t packet = (uint16_t)((y + 1) << SHIFT_PACKET_ADDR);
  packet |= ((led_matrix >> (LM_COLS * y)) & MASK_PACKET_DATA);
  send_packet(packet);
}

static void send_packet(uint16_t packet) {
  while (!LL_SPI_IsActiveFlag_TXE(MAX7219_SPI_CHANNEL))
      ;
  CHIP_SELECT(MAX7219_CS);
  LL_SPI_TransmitData16(MAX7219_SPI_CHANNEL, packet);
  CHIP_UNSELECT(MAX7219_CS);
}
