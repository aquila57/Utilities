/* getprime.c - Find highest prime  Version 0.1.0                    */
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

/* This program finds the highest prime number equal or less than    */
/* the input parameter.                                              */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s N\n", pgm);
   fprintf(stderr,"Where N is 3 to 999999999\n");
   fprintf(stderr,"Example: %s 100\n", pgm);
   exit(1);
   } /* putstx */

int main(int argc, char **argv)
   {
   int prm;
   int prmroot;
   double dblroot;
   if (argc != 2) putstx(*argv);
   prm = atoi(*(argv+1));
   if (prm < 3 || prm > 999999999)
      {
      fprintf(stderr,"%s: parameter N "
         "is out of range\n", *argv);
      putstx(*argv);
      } /* out of range */
   prm = (prm - 1) | 1;
   dblroot = floor(sqrt((double) prm));
   prmroot = (int) dblroot;
   if (prmroot*prmroot == prm) prm -= 2;
   while (prm > 3)
      {
      int i;
      i = 3;
      while (1)
         {
	 if (prm%i == 0) break;
	 if (i > prmroot) break;
	 i += 2;
	 } /* for each divisor */
      if (i > prmroot) break;
      prm -= 2;
      } /* for each prime candidate */
   printf("%d\n", prm);
   return(0);
   } /* main */
