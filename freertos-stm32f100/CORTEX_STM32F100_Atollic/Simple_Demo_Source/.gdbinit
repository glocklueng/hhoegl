# Note, start this within Emacs as
#  M-x arm-elf-gdb --annotate=3

# This .gdbinit file is in the home directory.  It is always read
# first.  Here we put only common settings.  We also use a .gdbinit
# file in each working directory which is specific to the particular
# project.

set complaints 1

set output-radix 0x10
set input-radix 0x10

set endian little

dir .
set prompt (arm-gdb) 

file demo.elf

# connect to openOCD running on gdb port 3333
target remote localhost:3333

# Set a breakpoint
b _start

monitor reset halt

# cont
