/*
jd2gr.c  Convert Julian Day to Gregorian  Version 0.1.0
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

/* The algorithm for converting Julian Day to Gregorian is */
/* documented in:                                          */

/* https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html */

#include "crsclk.h"

void jd2gr(xxfmt *xx)
   {
   double dblyr;
   double dblmon;
   double dblday;
   double que;
   double zee;
   double exx;
   double www;
   double ayy;
   double bee;
   double cee;
   double dee;
   double eee;
   double eff;
   que = xx->jd + 0.5;
   zee = floor(que);
   www = floor((zee - 1867216.25) / 36524.25);
   exx = floor(www / 4.0);
   ayy = floor(zee + 1.0 + www - exx);
   bee = floor(ayy + 1524.0);
   cee = floor((bee - 122.1) / 365.25);
   dee = floor(365.25 * cee);
   eee = floor((bee - dee) / 30.6001);
   eff = floor(30.6001 * eee);
   dblday = bee - dee - eff + (que - zee);
   dblmon = eee - 1.0;
   if (dblmon > 12.0) dblmon -= 12.0;
   if (dblmon < 3.0)
      dblyr = cee - 4715.0;
   else
      dblyr = cee - 4716.0;
   xx->year  = (int) floor(dblyr);
   xx->month = (int) floor(dblmon);
   xx->day   = (int) floor(dblday);
   } /* jd2gr */
