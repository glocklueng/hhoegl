
CC = arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as

ASFLAGS=-g
LDFLAGS=-Tstm32f100.ld -Llib

VPATH = $(LIBSRCDIR)

ROOTDIR = /home/hhoegl/STM32-Baustelle/STM32F10x_StdPeriph_Lib_V3.5.0

# startup file fuer gcc in das aktuelle Verzeichnis kopiert
STARTUP = $(ROOTDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_ld_vl.s

LIBSRCDIR = $(ROOTDIR)/Libraries/STM32F10x_StdPeriph_Driver/src
LIBINCDIR = $(ROOTDIR)/Libraries/STM32F10x_StdPeriph_Driver/inc

CMSISSRC = $(ROOTDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
CMSISCORE = $(ROOTDIR)/Libraries/CMSIS/CM3/CoreSupport/

LIBSRC = misc.c stm32f10x_crc.c stm32f10x_flash.c  stm32f10x_pwr.c   stm32f10x_tim.c stm32f10x_adc.c  stm32f10x_dac.c     stm32f10x_fsmc.c   stm32f10x_rcc.c   stm32f10x_usart.c stm32f10x_bkp.c  stm32f10x_dbgmcu.c  stm32f10x_gpio.c   stm32f10x_rtc.c   stm32f10x_wwdg.c stm32f10x_can.c  stm32f10x_dma.c     stm32f10x_i2c.c    stm32f10x_sdio.c stm32f10x_cec.c  stm32f10x_exti.c    stm32f10x_iwdg.c   stm32f10x_spi.c  missing.c

LIBOBJS = $(LIBSRC:.c=.o)

CFLAGS = -g -O1 -c -fno-common -mcpu=cortex-m3 -mthumb -DSTM32F10X_LD_VL=1 -DUSE_STDPERIPH_DRIVER=1 -I$(LIBINCDIR) -I$(CMSISSRC) -I$(CMSISCORE) -I.

all: $(LIBOBJS) libst.a

test:
	@echo $(LIBOBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	

libst.a: $(LIBOBJS)
	$(LD) -o libst.a $(LDFLAGS) $(LIBOBJS)

clean:
	rm -f libst.a *.o *~
