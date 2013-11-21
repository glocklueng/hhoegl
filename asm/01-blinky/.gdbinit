# - .gdbinit -

# This file is executed on startup by arm-none-eabi-gdb [--tui].  It is a
# project specific initialization file. You may also have a .gdbinit in your
# home directory for common settings.
#
# The GDB debugger documentation is available online by typing
# "info gdb". Be sure to install the package "gdb-doc".

set complaints 1

set output-radix 0x10
set input-radix 0x10

set endian little

dir .

set prompt (arm-gdb) 

file led-stm32.elf

set remote hardware-breakpoint-limit 6
set remote hardware-watchpoint-limit 4

# connect to openOCD running on gdb port 3333
target remote localhost:3333

# Set a breakpoint
b _start

monitor reset halt
#continue

define myflash
monitor halt
monitor poll
monitor flash erase_sector 0 0 0
monitor flash write_bank 0 led-stm32.bin 0
monitor reset init
continue
end

define testfunc 
printf "Parameter 1 is %d\n", $arg0
end

document testfunc
Just a test function. Run as "testfunc <n>". The
input number is interpreted as hex!
end
