
# gcc-arm-embedded
CC = arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
ECTAGS = ctags

ASFLAGS=-g
LDFLAGS=-Tstm32f100.ld -Llib

VPATH = $(LIBSRCDIR)

ROOTDIR = /home/hhoegl/STM32/STM32F10x_StdPeriph_Lib_V3.5.0

# startup file fuer gcc in das aktuelle Verzeichnis kopiert
# STARTUP = $(ROOTDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_ld_vl.s

LIBSRCDIR = $(ROOTDIR)/Libraries/STM32F10x_StdPeriph_Driver/src
LIBINCDIR = $(ROOTDIR)/Libraries/STM32F10x_StdPeriph_Driver/inc

CMSISSRC = $(ROOTDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
CMSISCORE = $(ROOTDIR)/Libraries/CMSIS/CM3/CoreSupport/

ASSRC = startup_stm32f10x_md_vl.s
ASOBJ = $(ASSRC:.s=.o)

SRCS = main.c stm32f10x_it.c system_stm32f10x.c 
OBJS = $(SRCS:.c=.o) $(ASOBJ)

CFLAGS = -g -O1 -c -fno-common -mcpu=cortex-m3 -mthumb -DSTM32F10X_MD_VL=1 -DUSE_STDPERIPH_DRIVER=1 -I$(LIBINCDIR) -I$(CMSISSRC) -I$(CMSISCORE) -I.

all: main

main: $(OBJS)
	$(CC) -o main $(OBJS) -Tstm32f100.ld -L. -lst

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	
# exuberant ctags!
tags:
	$(ECTAGS) $(ROOTDIR)/Libraries/STM32F10x_StdPeriph_Driver/inc/* $(ROOTDIR)/Libraries/STM32F10x_StdPeriph_Driver/src/* *.c *.h *.s

clean:
	rm -f main *.o tags *~
