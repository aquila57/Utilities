# Hexadecimal_Dump

This directory contains a utility program to print a file
in hexadecimal dump format.  Each line in the dump is 16
bytes long.  Each line contains the offset of the line,
the data in hexadecimal format, and the data in ASCII format.

## Windows

hx.exe works under Windows.

Example:

echo "The quick brown fox jumps over the lazy dog" | hx.exe
