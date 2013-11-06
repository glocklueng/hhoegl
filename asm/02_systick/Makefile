
# GNU assembler version 2.20.51 (arm-none-eabi) using BFD version 
# (Sourcery G++ Lite 2011.03-42) 2.20.51.20100809
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
RM = rm
SRC = led_systick

all:
		./preasm.tcl $(SRC).asm $(SRC).s 
		$(AS) -mcpu=cortex-m3 -mthumb -mapcs-32 -gstabs -ahls=$(SRC).lst -o $(SRC).o $(SRC).s
		$(LD) -v -T stm32.ld -nostartfiles -o $(SRC).elf $(SRC).o
		$(OC) -O binary $(SRC).elf  $(SRC).bin

tags:
		ctags-exuberant $(SRC).asm

clean:
		$(RM) -f $(SRC).s $(SRC).bin $(SRC).elf $(SRC).lst *.o *~
