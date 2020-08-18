/* rndpsw.c - Create random password  Version 0.1.0                  */
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

/* This program creates a random 8 character password                */
/* based on three seed parameters.                                   */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "etaus.h"

void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s N N N\n", pgm);
   fprintf(stderr,"Where N is 3 to 1048575\n");
   fprintf(stderr,"Example: %s 23 57 93\n", pgm);
   exit(1);
   } /* putstx */

int main(int argc, char **argv)
   {
   int i;
   int sd1,sd2,sd3;
   int indx;
   int lmt;
   char *p,*last;
   char alfabet[256];
   etfmt *et;
   if (argc != 4) putstx(*argv);
   sd1 = atoi(*(argv+1)) | 1;
   sd2 = atoi(*(argv+2)) | 1;
   sd3 = atoi(*(argv+3)) | 1;
   if (sd1 < 3 || sd1 > 1048575)
      {
      fprintf(stderr,"rndpsw: first seed is "
         "out of range\n");
      putstx(*argv);
      exit(1);
      } /* out of range */
   if (sd2 < 3 || sd2 > 1048575)
      {
      fprintf(stderr,"rndpsw: second seed is "
         "out of range\n");
      putstx(*argv);
      exit(1);
      } /* out of range */
   if (sd3 < 3 || sd3 > 1048575)
      {
      fprintf(stderr,"rndpsw: third seed is "
         "out of range\n");
      putstx(*argv);
      exit(1);
      } /* out of range */
   if (sd1 == sd2 || sd1 == sd3 || sd2 == sd3)
      {
      fprintf(stderr,"rndpsw: please create more "
         "unique parameters\n");
      putstx(*argv);
      exit(1);
      } /* duplicate seeds */
   et = (etfmt *) etausstrt(sd1,sd2,sd3);
   strcpy(alfabet,"abcdefghijklmnopqrstuvwxyz");
   strcat(alfabet,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   strcat(alfabet,"0123456789");
   p = (char *) alfabet;
   lmt = 62;
   last = (char *) alfabet + lmt - 1;
   i = 8;
   while (i--)
      {
      indx = etausint(et,lmt--);
      p = (char *) alfabet + indx;
      printf("%c", *p);
      *p = *last;
      *last-- = 0xff;
      } /* for each random digit no replacement */
   printf("\n");
   free(et->state);
   free(et);
   return(0);
   } /* main */
