#!/usr/bin/make

# xdgtclk Version 0.3.0 - Compile Digital Clock for X Windows.
# Copyright (C) 2003-2021  https://github.com/aquila57

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

################################################################

# xdgtclk Version 0.2.0 - Compile Digital Clock for X Windows.
# Copyright (C) 2003-2008  dondalah721@yahoo.com (Dondalah)

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

# Debian font
XFONT=\"12x24\"

# Uncomment if you want the xdgtclk window
# to appear in the upper left corner of
# your virtual desktop
# UPPERLEFT=-DUPPERLEFT

CC=gcc

CFLAGS=-I/usr/X11R6/include/X11 \
	-DFONT=$(XFONT) \
	$(UPPERLEFT)

LDFLAGS=-L/usr/X11R6/lib -lX11

xdgtclk:	xdgtclk.c xdgtclk.mak
	$(CC) $(CFLAGS) xdgtclk.c -o xdgtclk $(LDFLAGS)

clean:
	rm -f xdgtclk
