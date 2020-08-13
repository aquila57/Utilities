/*
crsclk.h  Header file for a digital clock  Version 0.1.0
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

/* clock.h */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <curses.h>

/* global structure */

typedef struct xxstruct {
   int year;
   int month;
   int day;
   int hour;
   int minute;
   int second;
   time_t now;                 /* current date and time */
   clock_t clk;                /* current number of ticks */
   struct tms t;               /* structure used by times() */
   struct tm *loctime;         /* structure used by localtime() */
   int ampm;                   /* 0 = am, 1 = pm */
   int dst;         /* CST vs DST 0 = standard, 1 = daylight */
   int dow;                   /* day of week 0-6 */
   int ytd;                   /* day of year 0-365 */
   double jd;
   char *font;
   } xxfmt;

/* seconds per day constant */

#define SPD (86400.0)

void gethms(xxfmt *xx);
void tm2jd(xxfmt *xx);
void jd2gr(xxfmt *xx);
void jd2tm(xxfmt *xx);
double jdcalc(int year, int month, int day,
   int hour, int minute, int second);
int getbyte(void);
void ldfont(xxfmt *xx);
void initcrss(void);
void putscr(xxfmt *xx);
