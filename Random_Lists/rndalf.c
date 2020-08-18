/* rndalf.c - Print random alphabet  Version 0.1.0                   */
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "etaus.h"

int main(void)
   {
   int i;
   int indx;
   int lmt;
   char *p,*last;
   char alfabet[256];
   etfmt *et;
   et = (etfmt *) etausinit();
   strcpy(alfabet,"abcdefghijklmnopqrstuvwxyz");
   p = (char *) alfabet;
   lmt = 26;
   last = (char *) alfabet + lmt - 1;
   i = lmt;
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
