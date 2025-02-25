#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (96U * 1024U) /* 96K */
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define SP_INIT_ADDR (SRAM_END) /* i.e. stack pointer */

#define VECTOR_TABLE_SIZE 100 /* 0x0190 / 4 */

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss; /* linker symbols */

void main(void);

void reset_handler(void);
/* we don't need the other handlers,
 * so just alias to default handler which is an infinite loop */
void default_handler(void);
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void mm_fault_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handler(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

uint32_t isr_vector[VECTOR_TABLE_SIZE]
  __attribute__((section(".isr_vector")))
  = {SP_INIT_ADDR,
     (uint32_t)&reset_handler,
     (uint32_t)&nmi_handler,
     (uint32_t)&hard_fault_handler,
     (uint32_t)&mm_fault_handler,
     (uint32_t)&bus_fault_handler,
     (uint32_t)&usage_fault_handler,
     0,
     0,
     0,
     0,
     0,
     (uint32_t)&svcall_handler,
     (uint32_t)&debug_monitor_handler,
     0,
     (uint32_t)&pendsv_handler,
     (uint32_t)&systick_handler};

void default_handler(void)
{
  for (;;);
}

void reset_handler(void)
{
  uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  /* not completely sure why we start at _etext and not _sdata here */
  uint8_t *flash_data = (uint8_t *)&_etext;
  uint8_t *sram_data = (uint8_t *)&_sdata;

  /* copy .data from flash to sram */
  for(uint32_t i = 0; i < data_size; i++)
    {
      sram_data[i] = flash_data[i];
    }

  uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  uint8_t *bss = (uint8_t *)&_sbss;
  
  /* zero out bss */
  for(uint32_t i = 0; i < bss_size; i++)
    {
      bss[i] = 0;
    }

  main();
}
