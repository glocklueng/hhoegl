
# GNU assembler version 2.20.51 (arm-none-eabi) using BFD version 
# (Sourcery G++ Lite 2011.03-42) 2.20.51.20100809
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
RM = rm

all:
		./preasm.tcl led-stm32.asm led-stm32.s
		$(AS) -mcpu=cortex-m3 -mthumb -mapcs-32 -gstabs -ahls=led-stm32.lst -o led-stm32.o led-stm32.s
		$(LD) -v -T stm32.ld -nostartfiles -o led-stm32.elf led-stm32.o
		$(OC) -O binary led-stm32.elf  led-stm32.bin

clean:
		$(RM) -f led-stm32.s
