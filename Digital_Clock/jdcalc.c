/* https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

double jdcalc(int year, int month, int day,
   int hour, int min, int sec)
   {
   double dblyr;
   double dblmon;
   double dblday;
   double dblhr;
   double dblmin;
   double dblsec;
   double ayy;
   double bee;
   double cee;
   double dee;
   double eee;
   double eff;
   double julsec;
   double julday;
   dblyr  = (double) year;
   dblmon = (double) month;
   if (dblmon < 3.0)
      {
      dblyr -= 1.0;
      dblmon += 12.0;
      } /* if jan or feb */
   dblday = (double) day;
   dblhr  = (double) hour;
   dblmin = (double) min;
   dblsec = (double) sec;
   ayy = floor(dblyr / 100.0);
   bee = floor(ayy / 4.0);
   cee = 2.0 - ayy + bee;
   dee = dblday;
   eee = floor(365.25 * (dblyr + 4716.0));
   eff = floor(30.6001 * (dblmon + 1.0));
   julday = cee + dee + eee + eff - 1524.5;
   /*******************************************************************/
   julsec = julday * 86400.0;
   julsec += 3600.0 * dblhr;
   julsec += 60.0 * dblmin;
   julsec += (double) dblsec;
   julsec += 3600.0 * 6.0;   /* convert from CST to UTC */
   julday = julsec / 86400.0;
   return(julday);
   } /* jdcalc */
