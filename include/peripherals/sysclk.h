#ifndef SYSCLK_H
#define SYSCLK_H

/**
 * Configure sysclk to use maximum speed of 84MHz,
 * AHB1 84MHz,
 * APB1 42MHz bus, 84MHz timer,
 * APB2 84MHz bus and timer
 **/
void sysclk_init(void);

#endif
