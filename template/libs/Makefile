include ../Makefile.common
LIBS+=libstm32.a
CFLAGS+=-c

all: libs

libs: $(LIBS)

libstm32.a:

		@echo -n "Building $@ ..."
		@cd $(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates && \
			$(CC) $(CFLAGS) \
			system_stm32f10x.c
		@cd $(STMLIB)/STM32F10x_StdPeriph_Driver/src && \
			$(CC) $(CFLAGS) \
				-D"assert_param(expr)=((void)0)" \
				-I../../CMSIS/Include \
				-I../../CMSIS/Device/ST/STM32F10x/Include \
				-I../inc \
				*.c
#		@cd $(STMLIB)/STM32_USB-FS-Device_Driver/src && \
			$(CC) $(CFLAGS) \
				-D"assert_param(expr)=((void)0)" \
				-I../../CMSIS/Include \
				-I../../CMSIS/Device/ST/STM32F10x/Include \
				-I../inc \
				*.c
		@$(AR) cr $(LIBDIR)/$@ \
			$(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates/system_stm32f10x.o \
			$(STMLIB)/STM32F10x_StdPeriph_Driver/src/*.o \
#			$(STMLIB)/STM32_USB-FS-Device_Driver/src/*.o
		@echo "done."

.PHONY: libs clean

clean:

		rm -f $(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates/system_stm32f10x.o
		rm -f $(STMLIB)/STM32F10x_StdPeriph_Driver/src/*.o
		rm -f $(STMLIB)/STM32_USB-FS-Device_Driver/src/*.o
		rm -f $(LIBS)

