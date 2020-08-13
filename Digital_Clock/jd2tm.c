/*
jd2tm.c  Convert Julian Day fraction to hh:mm:ss  Version 0.1.0
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

void jd2tm(xxfmt *xx)
   {
   double dblday;
   double daysec;
   double frac;
   double dblhr;
   double dblmin;
   double dblsec;
   double hrfrac;
   double minfrac;
   dblday = floor(xx->jd);
   frac = xx->jd - dblday;
   if (frac < 0.5) xx->ampm = 1;    /* PM */
   else xx->ampm = 0;    /* AM */
   daysec = floor(frac * 86400 + 0.1);
   dblhr = floor(daysec / 3600.0);
   if (xx->ampm == 0 && dblhr >= 12.0) dblhr -= 12.0;
   if (xx->ampm == 1 && dblhr < 12.0) dblhr += 12.0;
   hrfrac = (daysec / 3600.0) - dblhr;
   dblmin = floor(hrfrac * 60);
   if (dblmin >= 720.0) dblmin -= 720.0;
   minfrac = (hrfrac * 60.0) - dblmin;
   dblsec = floor(minfrac * 60.0);
   if (dblsec >= 43200.0) dblsec -= 43200.0;
   xx->hour = (int) dblhr;
   if (xx->dst)
      {
      xx->hour += 1;
      if (xx->hour > 23) xx->hour -= 24;
      } /* if daylight saving time */
   xx->minute = (int) dblmin;
   xx->second = (int) dblsec;
   } /* jd2tm */
