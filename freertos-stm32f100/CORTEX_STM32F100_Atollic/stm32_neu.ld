/*
	this linker script is from
  http://www.triplespark.net/elec/pdev/arm/stm32.html
  Nov 2012
*/

/* This will work with STM32 type of microcontrollers.    *
 * The sizes of RAM and flash are specified smaller than  *
 * what most of the STM32 provide to ensure that the demo *
 * program will run on ANY STM32.                         */
MEMORY
{
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
    rom (rx)  : ORIGIN = 0x00000000, LENGTH = 128K
}

SECTIONS
{
    .  = 0x0;         /* From 0x00000000 */
    .text :
    {
        *(vectors)    /* Vector table */
        *(.text)      /* Program code */
        *(.rodata)    /* Read only data */
    } >rom

    .  = 0x20000000;  /* From 0x20000000 */
    .data :
    {
        *(.data)      /* Data memory */
    } >ram AT > rom

    .bss :
    {
        *(.bss)       /* Zero-filled run time allocate data memory */
    } >ram AT > rom

    /* __stack_end__ = 0x20010000; */  /* --hh */
    __stack_end__ = 0x20002000;  /* STMF100RBT6 has 8K RAM --hh */

}
