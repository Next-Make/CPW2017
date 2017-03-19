#!/bin/bash

name=$1

if [ -z "$name" ]
then
    echo "Missing name of file"
    exit 1
fi

# Compile the code
avr-gcc -std=c99 -g -DF_CPU=8000000 -Wall -O1 -Werror -Wextra -mmcu=attiny88 -Wa,-ahlmns=$name.lst -c -o $name.o $name.c
avr-gcc -std=c99 -g -DF_CPU=8000000 -Wall -O1 -Werror -Wextra -mmcu=attiny88 -o $name.elf $name.o
avr-objcopy -j .text -j .data -O ihex $name.elf $name.hex

# Program the compiled code onto board
avrdude -p t88 -c avrisp2 -B 1 -v -U flash:w:$name.hex -U lfuse:w:0xEE:m