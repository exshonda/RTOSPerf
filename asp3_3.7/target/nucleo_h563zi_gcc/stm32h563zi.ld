MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
  SRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 655K
}

ENTRY(_kernel_start)

SECTIONS
{
	.text :
	{
		__text = .;
		*(.vector)
		*(.text)
		*(.text.*)
	} > FLASH
	_etext = .;
	PROVIDE(etext = .);

	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
		KEEP(*(.empty.*))
	} > FLASH

	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
	__exidx_start = .;
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	__exidx_end = .;

	. = ALIGN(4);
	__idata_start = .;
	.data : 
	{
		__data_start = .;
		*(.data)
		*(.data.*)
	} > SRAM AT > FLASH
	__idata_end = __idata_start + SIZEOF(.data);
	_edata = .;
	PROVIDE(edata = .);

	. = ALIGN(8);
	.bss :
	{
		__bss_start  = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
	} > SRAM
	. = ALIGN(4);
	_end = .;
	__bss_end = .;
	PROVIDE(end = .);
}
