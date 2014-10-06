
CC = arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as

ASFLAGS=-g
LDFLAGS=-Tstm32f100.ld -Llib

VPATH = $(LIBSRCDIR)

ROOTDIR = ../../../stm32-stdperiph-3.5.0

# startup file fuer gcc in das aktuelle Verzeichnis kopiert
# STARTUP = $(ROOTDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md_vl.s
STARTUP = startup_stm32f10x_md_vl.s

LIBSRCDIR = $(ROOTDIR)/src
LIBINCDIR = $(ROOTDIR)/include
UTILINCDIR = $(ROOTDIR)/Utilities/STM32_EVAL/STM32_Discovery/

CMSISSRC = $(ROOTDIR)/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
CMSISCORE = $(ROOTDIR)/CMSIS/CM3/CoreSupport/

ASSRC = startup_stm32f10x_md_vl.s
ASOBJ = $(ASSRC:.s=.o)

SRCS = main.c system_stm32f10x.c 
OBJS = $(SRCS:.c=.o) $(ASOBJ)

CFLAGS = -g -O1 -c -fno-common -mcpu=cortex-m3 -mthumb -DSTM32F10X_MD_VL=1 -DUSE_STDPERIPH_DRIVER=1 -I$(LIBINCDIR) -I$(CMSISSRC) -I$(CMSISCORE) -I$(UTILINCDIR)   -I.

all: main

main: $(OBJS)
	$(CC) -o main $(OBJS) -Tstm32f100.ld -L../../../stm32-stdperiph-3.5.0/lib/ -lst

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	
clean:
	rm -f main *.o *~
