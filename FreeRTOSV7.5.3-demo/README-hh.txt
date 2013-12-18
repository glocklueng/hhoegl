Beispiel auf FreeRTOS7.5.3 auf Linux mit gcc-arm-embedded angepasst

Dem Linker ist das 8K grosse RAM um etwa 1K zu klein:

arm-none-eabi-gcc -o main main.o system_stm32f10x.o STM32vldiscovery.o croutine.o tasks.o list.o queue.o timers.o port.o heap_1.o startup_stm32f10x_md_vl.o -Tstm32f100.ld -L/home/hhoegl/Dienste/git/stm32-at-hsa/stm32-stdperiph-3.5.0/lib/ -lst
/opt/gcc-arm-none-eabi-4_7-2013q3/bin/../lib/gcc/arm-none-eabi/4.7.4/../../../../arm-none-eabi/bin/ld: main section `.bss' will not fit in region `RAM'
/opt/gcc-arm-none-eabi-4_7-2013q3/bin/../lib/gcc/arm-none-eabi/4.7.4/../../../../arm-none-eabi/bin/ld: region `RAM' overflowed by 1124 bytes
collect2: error: ld returned 1 exit status
make: *** [main] Error 1

Wenn man im Linker Skript die RAM Groesse testweise auf 16K erhoeht, wird
die ausfuehrbare Datei erzeugt. Sie verbraucht erwa 1K in der .bss, und etwa
8K in der .data Sektion.

Wenn man die Heap-Groesse auf 4*1024 Byte reduziert (FreeRTOSConfig.h), 
dann kompiliert das Programm.

H. Hoegl, 2013-11-18 
