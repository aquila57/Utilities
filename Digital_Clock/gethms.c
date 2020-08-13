/* gethms.c - Get hours minutes seconds  Version 0.1.0               */
/* Copyright (C) 2020 aquila57 at github.com                         */

/* This program is free software; you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License as    */
/* published by the Free Software Foundation; either version 2 of    */
/* the License, or (at your option) any later version.               */

/* This program is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/* GNU General Public License for more details.                      */

/* You should have received a copy of the GNU General Public License */
/* along with this program; if not, write to:                        */

   /* Free Software Foundation, Inc.                                 */
   /* 59 Temple Place - Suite 330                                    */
   /* Boston, MA 02111-1307, USA.                                    */

#include "crsclk.h"

void gethms(xxfmt *xx)
   {
   /* get clock ticks since boot                          */
   xx->clk = times(&xx->t);
   /* get date & time, in sec since 1970.1.1 00:00:00     */
   xx->now = time(NULL);
   /* populate the tm structure loctime */
   xx->loctime = localtime(&xx->now);
   xx->hour = xx->loctime->tm_hour;
   if (xx->hour > 23) xx->hour -= 24;
   if (xx->hour > 11)
      {
      xx->ampm = 1;
      } /* if PM */
   else
      {
      xx->ampm = 0;
      } /* if AM */
   xx->minute = xx->loctime->tm_min;
   xx->second = xx->loctime->tm_sec;
   xx->ytd    = xx->loctime->tm_yday;
   xx->dow    = xx->loctime->tm_wday;
   xx->dst    = xx->loctime->tm_isdst;
   } /* gethms */
