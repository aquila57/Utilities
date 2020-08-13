/*
ldfont.c Load font from stdin  Version 0.1.0
Copyright (C) 2020  aquila57 at github.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to:

	Free Software Foundation, Inc.
	59 Temple Place - Suite 330
	Boston, MA  02111-1307, USA.
*/

#include "tim.h"

void ldfont(xxfmt *xx)
   {
   char *p,*q;
   int i;
   i = 0;
   p = (char *) xx->font;
   q = (char *) xx->font + (11 * 9 * 15);
   while (p < q)
      {
      int ch;
      i++;
      ch = getbyte();
      if (ch == EOF) break;
      if (i > 9)
         {
	 while (ch != '\n')
	    {
	    ch = getbyte();
	    if (ch == EOF) break;
	    } /* read to eol */
         if (ch == EOF) break;
	 i = 0;
	 } /* if font > 9 wide */
      else
	 {
         *p++ = (char) ch;
	 } /* else valid pixel */
      if (ch == EOF) break;
      } /* for each pixel in font */
   } /* ldfont */
