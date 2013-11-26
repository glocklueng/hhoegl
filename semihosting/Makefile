

CFLAGS  =  -I./ -fno-common -g  -mcpu=cortex-m3 -mthumb
LDFLAGS  = -Tstm32f100.ld -nostartfiles --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group 
CPFLAGS = -Obinary
ODFLAGS = -S


.PHONY: bin


all: main.bin main.lst

main.elf: main.c
	arm-none-eabi-gcc $(CFLAGS) -o main.elf main.c  $(LDFLAGS)

main.bin: main.elf
	arm-none-eabi-objcopy -Obinary main.elf main.bin

main.lst: main.elf
	arm-none-eabi-objdump -S main.elf > main.lst

clean:
	rm -f main.bin main.elf main.lst *~
