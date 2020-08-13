/*
tim.c - Curses Timer, 9x15 ASCII font  Version 0.1.0
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

#include "tim.h"

void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s seconds\n", pgm);
   fprintf(stderr,"Example: %s 300\n", pgm);
   exit(1);
   } /* putstx */

int main(int argc, char **argv)
   {
   int ch;                 /* ASCII character to display */
   char *p,*q;
   char blnk[256];
   xxfmt *xx;              /* global structure */
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx structure\n");
      exit(1);
      } /* out of memory */
   if (argc != 2) putstx(*argv);
   xx->endsec = atoi(*(argv+1));
   if (xx->endsec < 1 || xx->endsec >= 86400)
      {
      fprintf(stderr,"main: invalid #seconds\n");
      exit(1);
      } /* invalid #seconds parameter */
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
   while(xx->endsec > 0)               /* infinite loop     */
      {
      double dblwt;
      double dblhrs;
      double dblhh;
      double dblmin;
      double dblmm;
      double dblsec;
      double dblss;
      dblwt = (double) xx->endsec;
      if (dblwt >= SPD)
	 {
         dblwt /= SPD;
	 dblwt -= floor(dblwt);
	 } /* if more than 24 hrs */
      else dblwt /= SPD;
      dblhrs  = dblwt * 24.0;
      dblhh   = floor(dblhrs + 0.0001);
      dblmin  = dblhrs - floor(dblhrs);
      dblmin *= 60.0;
      dblmm   = floor(dblmin + 0.0001);
      dblsec  = dblmin - floor(dblmin);
      dblsec *= 60.0;
      dblss   = floor(dblsec + 0.0001);
      if (dblss >= 60) dblss -= 60.0;
      xx->hour   = (int) dblhh;
      xx->minute = (int) dblmm;
      xx->second = (int) dblss;
      putscr(xx);
      refresh();         /* display the new screen */
      ch = getch();      /* test keyboard for interrupt */
      /* quit if 'escape' or 'q' */
      if (ch == 0x1b) /* ESCAPE */
         break;
      if (ch == 'q') /* quit */
         break;
      sleep(1);
      dblhrs = dblmin = dblsec = dblhh = dblmm = dblss = 0.0;
      xx->endsec--;
      }
   endwin();           /* de-activate curses mode */
   while (1)
      {
      system("beep -f 55 -l 200 -r 5 -d 1000");
      sleep(60);
      } /* beep loop */
   free(xx->font);
   free(xx);
   return(0);          /* normal eoj */
   } /* main */
