
LDFLAGS = --specs=rdimon.specs -lc -lrdimon

.PHONY: bin

main: main.c
	arm-none-eabi-gcc $(LDFLAGS) -o main main.c

main.bin: main
	arm-none-eabi-objcopy -Obinary main main.bin

clean:
	rm -f main.bin main *~
