#  libcrc.mak - Compile crc32 library  Version 0.1.0
#  Copyright (C) 2020 aquila57 at github.com

#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of
#  the License, or (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to:

   #  Free Software Foundation, Inc.
   #  59 Temple Place - Suite 330
   #  Boston, MA 02111-1307, USA.

OBJ=bldtbl.o \
	crc32.o \
	crc32nxt.o \
	crc16.o \
	crc16nxt.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=

libcrc.a:			$(OBJ)
		rm -f libcrc.a
		ar r libcrc.a $(OBJ)

bldtbl.o:			bldtbl.c
		$(CC) $(CFLAGS) bldtbl.c

crc32.o:			crc32.c
		$(CC) $(CFLAGS) crc32.c

crc32nxt.o:			crc32nxt.c
		$(CC) $(CFLAGS) crc32nxt.c

crc16.o:			crc16.c
		$(CC) $(CFLAGS) crc16.c

crc16nxt.o:			crc16nxt.c
		$(CC) $(CFLAGS) crc16nxt.c

clean:
		rm -f $(OBJ) libcrc.a
