# clklib.mak - Compile clock subroutine library  Version 0.1.0
# Copyright (C) 2020   aquila57 at github.com

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to:

# 	Free Software Foundation, Inc.
# 	59 Temple Place - Suite 330
# 	Boston, MA  02111-1307, USA.

OBJ=tm2jd.o \
	jd2gr.o \
	jd2tm.o \
	jdcalc.o \
	gethms.o \
	getbyte.o \
	ldfont.o \
	initcrss.o \
	putscr.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=-lncurses -lm

libclk.a:			$(OBJ)
		rm -f libclk.a
		ar r libclk.a $(OBJ)

tm2jd.o:			tm2jd.c
		$(CC) $(CFLAGS) tm2jd.c

jd2gr.o:			jd2gr.c
		$(CC) $(CFLAGS) jd2gr.c

jd2tm.o:			jd2tm.c
		$(CC) $(CFLAGS) jd2tm.c

jdcalc.o:			jdcalc.c
		$(CC) $(CFLAGS) jdcalc.c

gethms.o:			gethms.c
		$(CC) $(CFLAGS) gethms.c

getbyte.o:			getbyte.c
		$(CC) $(CFLAGS) getbyte.c

ldfont.o:			ldfont.c
		$(CC) $(CFLAGS) ldfont.c

initcrss.o:			initcrss.c
		$(CC) $(CFLAGS) initcrss.c

putscr.o:			putscr.c
		$(CC) $(CFLAGS) putscr.c

clean:
		rm -f $(OBJ) libclk.a
