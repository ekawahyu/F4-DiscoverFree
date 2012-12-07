ENTRY(Reset_Handler)

MEMORY
{
	FLASH (rx)		: ORIGIN = 0x08000000, LENGTH = 1024K
	CCMRAM (xrw)	: ORIGIN = 0x10000000, LENGTH = 64K
	SRAM (xrw)		: ORIGIN = 0x20000000, LENGTH = 112K
	ETHRAM (xrw)	: ORIGIN = 0x2001C000, LENGTH = 16K
}

_estack = 0x2001C000;

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))

		. = ALIGN(4);
	} > FLASH
	
	.text :
	{
		. = ALIGN(4);
		_stext = .;
		*(.text*)
		*(.rodata*)
		KEEP (*(.init))
		KEEP (*(.fini))

		. = ALIGN(4);
		_etext = .;
	} > FLASH

	_sidata = .;
	.data : AT ( _sidata )
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data*)

		. = ALIGN(4);
		_edata = .;
	} > SRAM

	_sibss = .;
	.bss : AT ( _sibss )
	{
		. = ALIGN(4);
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} > SRAM

	__exidx_start = .;
	__exidx_end = .;
}
