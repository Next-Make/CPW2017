#!/bin/bash

# Compile the code
avr-gcc -std=c99 -g -DF_CPU=8000000 -Wall -O1 -Werror -Wextra -mmcu=attiny88 -Wa,-ahlmns=Next_Make_CPW_2017.lst -c -o Next_Make_CPW_2017.o Next_Make_CPW_2017.c
avr-gcc -std=c99 -g -DF_CPU=8000000 -Wall -O1 -Werror -Wextra -mmcu=attiny88 -o Next_Make_CPW_2017.elf Next_Make_CPW_2017.o
avr-objcopy -j .text -j .data -O ihex Next_Make_CPW_2017.elf Next_Make_CPW_2017.hex

# Program the compiled code onto board
avrdude -p t88 -c avrisp2 -B 1 -v -U flash:w:Next_Make_CPW_2017.hex -U lfuse:w:0xEE:m
