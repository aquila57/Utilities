/*
clk.c Curses Clock, large font  Version 0.1.0
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

/* initialize curses */
void initcrss()
   {
   initscr();
   cbreak();
   noecho();
   nonl();
   intrflush(stdscr,FALSE);
   nodelay(stdscr,TRUE);
   } /* initcrss */

void jd2gr(xxfmt *xx);

int main(void)
   {
   int ch;                 /* ASCII character to display */
   // int x;                  /* column coordinate 0-79 */
   // int y;                  /* row    coordinate 0-23 */
   char *p,*q;
   char str[256];
   char blnk[256];
   char weekday[256];
   char ascmonth[256];
   char alfmonth[32];
   char alfday[32];
   xxfmt *xx;
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx structure\n");
      exit(1);
      } /* out of memory */
   p = (char *) blnk;
   q = (char *) blnk + 255;
   while (p < q) *p++ = ' ';
   *p = '\0';
   p = (char *) weekday;
   q = (char *) weekday + 256;
   while (p < q) *p++ = ' ';
   p = (char *) weekday;
   strcpy(p,"Sunday");
   p += 16;
   strcpy(p,"Monday");
   p += 16;
   strcpy(p,"Tuesday");
   p += 16;
   strcpy(p,"Wednesday");
   p += 16;
   strcpy(p,"Thursday");
   p += 16;
   strcpy(p,"Friday");
   p += 16;
   strcpy(p,"Saturday");
   /*********************************************************/
   p = (char *) ascmonth;
   q = (char *) ascmonth + 256;
   while (p < q) *p++ = ' ';
   p = (char *) ascmonth;
   strcpy(p,"January");
   p += 16;
   strcpy(p,"February");
   p += 16;
   strcpy(p,"March");
   p += 16;
   strcpy(p,"April");
   p += 16;
   strcpy(p,"May");
   p += 16;
   strcpy(p,"June");
   p += 16;
   strcpy(p,"July");
   p += 16;
   strcpy(p,"August");
   p += 16;
   strcpy(p,"September");
   p += 16;
   strcpy(p,"October");
   p += 16;
   strcpy(p,"November");
   p += 16;
   strcpy(p,"December");
   /*********************************************************/
   initcrss();                         /* initialize curses mode */
   /* terminate loop with 'q' or 'escape'                   */
   while(1)                            /* infinite loop     */
      {
      tm2jd(xx);
      move(7,10);
      clrtoeol();
      strcpy(str,blnk);
      sprintf(str,"Julian Day  %f", xx->jd);
      addstr(str);
      /******************************************************/
      jd2gr(xx);
      gethms(xx);
      p = (char *) ascmonth + (xx->month << 4);
      strcpy(alfmonth,p);
      p = (char *) weekday + (xx->dow << 4);
      sprintf(alfday,"%s", p);
      move(10,10);
      clrtoeol();
      strcpy(str,blnk);
      sprintf(str,"%s, %s %d, %04d,  %02d:%02d:%02d",
         alfday, alfmonth, xx->day, xx->year,
         xx->hour, xx->minute, xx->second);
      addstr(str);
      /******************************************************/
      move(13,10);
      clrtoeol();
      strcpy(str,blnk);
      sprintf(str,"YTD %03d", xx->ytd);
      addstr(str);
      /******************************************************/
      move(0,0);
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
   return(0);          /* normal eoj */
   } /* main */
