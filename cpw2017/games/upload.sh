#!/bin/bash

name=$1

if [ -z "$name" ]
then
    echo "Missing name of file"
    exit 1
fi

# Compile the code
avr-gcc -std=c99 -g -DF_CPU=8000000 -Wall -O1 -Werror -Wextra -mmcu=attiny88 -Wa,-ahlmns=$name.lst -c -o $name.o $name.c || exit 1
avr-gcc -std=c99 -g -DF_CPU=8000000 -Wall -O1 -Werror -Wextra -mmcu=attiny88 -o $name.elf $name.o || exit 1
avr-objcopy -j .text -j .data -O ihex $name.elf $name.hex || exit 1

# Program the compiled code onto board
avrdude -p t88 -c usbtiny -B 1 -v -U flash:w:$name.hex -U lfuse:w:0xEE:m

# Display RAM consumption
avr-size -C --mcu=attiny88 $name.elf
