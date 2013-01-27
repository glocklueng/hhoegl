
# Summon ARM Toolchain
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
LDFLAGS =	-L/home/hhoegl/sat/arm-none-eabi/lib -lc

# Sourcery CodeBench Lite 2012.03-56
#CC = arm-none-eabi-gcc
#LD = arm-none-eabi-ld
#LDFLAGS =	-L/home/hhoegl/local/arm-2012.03/arm-none-eabi/lib -lc

# DIR = /home/hhoegl/freertos-stm32f100
DIR = /home/hhoegl/Dienste/git/stm32-at-hsa/freertos-stm32f100

VPATH = .:../../Source/:../Utilities/STM32_EVAL/STM32_Discovery/:../Libraries/STM32F10x_StdPeriph_Driver/src/:../../Source/portable/GCC/ARM_CM3/:../../Source/portable/MemMang/

CFLAGS = -c -mcpu=cortex-m3 -mthumb -gstabs -DSTM32F10X_LD -DUSE_FULL_ASSERT=1  -I$(DIR)/Source/include -I. -I../../Source/portable/GCC/ARM_CM3/ -I../../CORTEX_STM32F100_Atollic/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/ -I../../CORTEX_STM32F100_Atollic/Libraries/CMSIS/CM3/CoreSupport/ -I../../CORTEX_STM32F100_Atollic/Utilities/STM32_EVAL/STM32_Discovery/ -I../../CORTEX_STM32F100_Atollic/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x -I../Libraries/STM32F10x_StdPeriph_Driver/inc/ -I../Utilities/STM32_EVAL/STM32_Discovery

OBJS = STM32F10x_Startup.o main.o der-rest.o stm32f10x_it.o   port.o   list.o queue.o tasks.o croutine.o timers.o     system_stm32f10x.o      STM32vldiscovery.o      misc.o  stm32f10x_exti.o  stm32f10x_gpio.o  stm32f10x_rcc.o    heap_1.o \


all: $(OBJS)  out

%.o: %.c
		$(CC) $(CFLAGS) $<

out:
#		$(LD) -v -T ../stm32.ld -nostartfile -o demo.elf $(OBJS) $(LDFLAGS) -lc 
		$(LD) -v -T ../stm32_neu.ld -nostartfile -o demo.elf $(OBJS) $(LDFLAGS) -lc 
		
clean:
		rm -f *.o demo.elf *~

STM32F10x_Startup.o: STM32F10x_Startup.s
		arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mapcs-32 -ahls=STM32F10x_Startup.lst -o STM32F10x_Startup.o STM32F10x_Startup.s

bin:
		arm-none-eabi-objcopy -O binary demo.elf  demo.bin
