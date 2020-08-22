# CRC32

This repository contains a program to calculate 32 bit crc.
The crc generated matches the crc in pkzip.

crc - Print 32 bit crc of stdin file.

## CRC Library

The library of crc subroutines may be used in other programs.

bldtbl.c - build a crc table for crc calculation.

crc32.c - calculate the 32 bit crc of a memory buffer.

crc32nxt.c - calculate the 32 bit crc one block at a time.

crc16.c - calculate the 16 bit CCITT XMODEM-CRC of a block of data.

crc16nxt.c - calculate the 16 bit CCITT XMODEM-CRC one block at a time.
