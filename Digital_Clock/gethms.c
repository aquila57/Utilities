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

#include "tim.h"

void gethms(xxfmt *xx)
   {
   double totsec;
   double days;
   double dblhrs;
   double dblhh;
   double dblmin;
   double dblmm;
   double dblsec;
   double dblss;
   /* get clock ticks since boot                          */
   xx->clk = times(&xx->t);
   /* get date & time, in sec since 1970.1.1 00:00:00     */
   time(&xx->now);
   days = (double) xx->now / SPD;
   totsec = floor(((days - floor(days)) * SPD) + 0.01);
   dblhrs = (totsec / 3600.0) - 6.0;
   if (dblhrs < 0.0) dblhrs += 24.0;
   dblhh  = floor(dblhrs + 0.01);
   dblmin = (dblhrs - floor(dblhrs)) * 60.0;
   dblmm  = floor(dblmin + 0.01);
   dblsec = (dblmin - floor(dblmin)) * 60.0;
   dblss  = floor(dblsec + 0.01);
   xx->hour = (int) dblhh;
   xx->minute = (int) dblmm;
   if (dblss >= 60.0) dblss = 0;
   xx->second = (int) dblss;
   if (xx->hour >= 12) xx->ampm = 1;
   else xx->ampm = 0;
   } /* gethms */
