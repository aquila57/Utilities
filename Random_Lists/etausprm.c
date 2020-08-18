/* etausprm.c - random etaus parms Version 0.1.0                     */
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

/**********************************************************/
/* void etausprm(unsigned int *list)                      */
/* This subroutine calculates three large prime number    */
/* parameters at random.  The numbers may be used to      */
/* initialize etausstrt.c for regression testing.         */
/* This subroutine has one parameter, an array of         */
/* 3 integers.  The array is filled by etausprm.          */
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "etaus.h"

/* This subroutine fills lst[3] with three prime numbers */

void etausprm(unsigned int *lst)
   {
   int i;              /* counter for 3 prime numbers */
   etfmt *et;          /* etaus structure */
   /* etaus initialization based on date/time */
   et = (etfmt *) etausinit();   /* initialize etaus */
   i = 0;               /* initialize prime counter */
   while (i < 3)        /* for each prime number */
      {
      int prm;          /* candidate prime */
      int j;            /* divisor to test prime */
      int root;         /* square root of candidate */
      int rem;          /* remainder */
      double dblroot;   /* square root of candidate */
      /* select a 30 bit random number */ 
      /* that number will be tested to be prime */
      /* the number chosen is odd and greater than 2 */
      prm = (etauspwr(et,30) | 1) + 2;
      rem = 0;          /* initialize remainder */
      /***********************************************************/
      /* if number is not divisible by 1 or itself, it is prime. */
      /* if number is not prime, choose next higher odd number.  */
      /***********************************************************/
      while (!rem)      /* loop until prime number found */
         {
	 /* calculate square root of candidate */
         dblroot = sqrt((double) prm) + 1.0;
         root = (int) floor(dblroot);
         j = 3;
	 /* divide by all odd numbers <= square root */
         while (j <= root)     /* divide by odd numbers */
            {
	    rem = prm % j;     /* is candidate divisible? */
	    if (!rem) break;   /* yes, pick next candidate */
	    j += 2;            /* no, divide by next odd number */
	    } /* for each divisor */
	 prm += 2;             /* pick next candidate prime */
         } /* for each candidate prime */
      if (i == 1)
         {
	 if (lst[0] == lst[1])
	    {
	    continue;
	    } /* if duplicate prime numbers */
	 } /* if second prime */
      else if (i == 2)
         {
	 if (lst[0] == lst[1])
	    {
	    continue;
	    } /* if duplicate prime numbers */
	 else if (lst[0] == lst[2])
	    {
	    continue;
	    } /* if duplicate prime numbers */
	 else if (lst[1] == lst[2])
	    {
	    continue;
	    } /* if duplicate prime numbers */
	 } /* if second prime */
      lst[i] = prm - 2;        /* save valid prime number */
      i++;                     /* next number to save */
      } /* for each prime number chosen */
   free(et->state);            /* free etaus structure */
   free(et);                   /* free etaus structure */
   } /* etausprm subroutine */
