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

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"

/** CLOCK CONFIGURATION
 *
 * SYSCLK configured to use PLL with HSI as source
 * at max frequency 84MHz
 *
 * VCO input frequency must be between 1-2MHz,
 * preferably 2 to reduce jitter.
 *
 * VCO output frequency must be between 192-432MHz
 *
 * SYSCLK Source:               PLL (HSI -> PLL 84MHz)
 * PLL Source:                  HSI (16MHz)
 * PLL M Division Factor:       8 (16MHz / 8 = 2MHz)
 * PLL N Multiplication Factor: 168 (2MHz * 168 = 336MHz)
 * PLL P Division Factor:       4 (336MHz / 4 = 84MHz)
 **/
void sysclk_init(void) {
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8,
                              (uint32_t)168, LL_RCC_PLLP_DIV_4);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_PLL_Enable();
  while (!LL_RCC_PLL_IsReady())
    ;

  /* for 84MHz we need 5 wait state */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    ;

  /* 42MHz bus, 84MHz timer */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  /* 84MHz bus and timer */
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  
  SystemCoreClockUpdate();
}
