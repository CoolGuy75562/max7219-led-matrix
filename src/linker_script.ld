/* See:                                                                                */
/* https://github.com/kristianklein/stm32-without-cubeide/blob/part_1/linker_script.ld */
/* https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/        */

ENTRY(reset_handler)

MEMORY
{
	FLASH (rx): ORIGIN = 0x08000000, LENGTH = 512K
	SRAM (rwx): ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
	/* This tells the linker to keep the isr vector section, */
	/* even if it is not referenced in any code.             */
	.isr_vector :
	{
		KEEP(*(.isr_vector))
	} > FLASH

	.text :
	{
		. = ALIGN(4); /* Align section to word size */

		*(.text)
		*(.rodata)

		. = ALIGN(4);
		_etext = .;
	} > FLASH

	.data :
	{
		. = ALIGN(4);
		_sdata = .;
		
		*(.data)

		. = ALIGN(4);
		_edata = .;
	} > SRAM AT > FLASH /* Stored in flash and loaded into SRAM on startup */

	.bss :
	{
		. = ALIGN(4);
		_sbss = .;

		*(.bss)

		. = ALIGN(4);
		_ebss = .;
	} > SRAM
}
		
