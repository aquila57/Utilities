/*
tm2jd.c  Convert date/time to Julian Day  Version 0.1.0
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

#include "crsclk.h"

void tm2jd(xxfmt *xx)
   {
   double jd1970;
   double uxsec;
   double uctday;
   jd1970 = jdcalc(1970,01,01,0,0,0) - 0.25;
   /* get clock ticks since boot                          */
   xx->clk = times(&xx->t);
   /* get date & time, in sec since 1970.1.1 00:00:00     */
   time(&xx->now);
   uxsec = (double) xx->now;
   uctday = uxsec / SPD;
   xx->jd = jd1970 + uctday - 0.25;
   jd2gr(xx);
   jd2tm(xx);
   } /* tm2jd */
