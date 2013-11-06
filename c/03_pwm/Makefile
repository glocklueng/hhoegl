PREFIX = arm-none-eabi
CC      = $(PREFIX)-gcc
LD      = $(PREFIX)-ld -v
CP      = $(PREFIX)-objcopy
OD      = $(PREFIX)-objdump
  
CFLAGS  =  -I./ -c -fno-common -g -Os -mcpu=cortex-m3 -mthumb
LFLAGS  = -Tstm32.ld -nostartfiles
CPFLAGS = -Obinary
ODFLAGS = -S

PROJECT = pwm

all: test

clean:
	-rm -f *.lst *.o *.elf *.lst *.bin

test: $(PROJECT).elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(PROJECT).elf $(PROJECT).bin
	$(OD) $(ODFLAGS) $(PROJECT).elf > $(PROJECT).lst

$(PROJECT).elf: $(PROJECT).o stm32.ld 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o $(PROJECT).elf $(PROJECT).o    

$(PROJECT).o: $(PROJECT).c
	@ echo ".compiling"
	$(CC) $(CFLAGS) $(PROJECT).c


