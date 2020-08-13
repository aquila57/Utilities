/*
putscr.c - Paint the screen in curses  Version 0.1.0
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

void putscr(xxfmt *xx)
   {
   int row;
   char *p,*q;
   char *lnofst;
   char str[512];
   char ln[512];
   sprintf(str,"%02d:%02d:%02d",
      xx->hour, xx->minute, xx->second);
   row = 0;
   while (row < 15)
      {
      lnofst = (char *) ln;
      p = (char *) str;
      q = (char *) str + 8;
      while (p < q)
         {
	 int i;
	 int ch;
	 int ofst;
	 char *pxl;
	 ch = *p++ - '0';
	 ofst = (ch * 9 * 15) + (row * 9);
	 pxl = (char *) xx->font + ofst;
	 i = 9;
	 while (i--)
	    {
	    *lnofst++ = *pxl++;
	    } /* for each pixel in row */
	 } /* for each digit in time */
      *lnofst = '\0';
      move(5+row,2);
      clrtoeol();
      addstr(ln);
      row++;
      } /* for each row */
   } /* putscr */
