# Hexadecimal_Dump

This directory contains a utility program to print a file
in hexadecimal dump format.  Each line in the dump is 16
bytes long.  Each line contains the offset of the line,
the data in hexadecimal format, and the data in ASCII format.

## Subroutine

This directory also contains a subroutine to print a block
of memory in hexadecimal dump format.  Each line in the dump
is 16 bytes long.  Each line contains the offset of the line,
the data in hexadecimal format, and the data in ASCII format.

Name of subroutine: hxmem

Parameters: int len, unsigned char *mem

len is the length of the memory block.

mem is the address of the memory block.

## Test Program

tst.c is a program to test the hxmem subroutine.

There are no parameters for this test program.

Example:

tst
