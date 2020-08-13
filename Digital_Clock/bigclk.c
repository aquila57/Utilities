/*
bigclk.c Curses Clock, 9x15 ASCII font  Version 0.1.0
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

/* press 'q' to exit */

#include "crsclk.h"

int main(void)
   {
   int ch;                 /* ASCII character to display */
   char *p,*q;
   char blnk[256];
   xxfmt *xx;
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx structure\n");
      exit(1);
      } /* out of memory */
   xx->font = (char *) malloc(11 * 9 * 15);
   if (xx->font == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx->font\n");
      exit(1);
      } /* out of memory */
   ldfont(xx);
   p = (char *) blnk;
   q = (char *) blnk + 255;
   while (p < q) *p++ = ' ';
   *p = '\0';
   initcrss();                         /* initialize curses mode */
   /* terminate loop with 'q' or 'escape'                   */
   while(1)                            /* infinite loop     */
      {
      gethms(xx);
      putscr(xx);
      refresh();         /* display the new screen */
      ch = getch();      /* test keyboard for interrupt */
      /* quit if 'escape' or 'q' */
      if (ch == 0x1b) /* ESCAPE */
         break;
      if (ch == 'q') /* quit */
         break;
      sleep(1);
      }
   endwin();           /* de-activate curses mode */
   free(xx->font);
   free(xx);
   return(0);          /* normal eoj */
   } /* main */
