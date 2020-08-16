/* etausinit.c - Extended taus initialization Version 2.0.0          */
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

/***************************************************************/
/* This version of etaus is based on:                          */
/* http://www.github.com/aquila62/etaus                        */
/* The size of the Bays-Durham shuffle has been increased.     */
/***************************************************************/

/* This is the initialization routine for extended taus    */
/* This initialization routine is based on date/time/ticks */
/* The caller is welcome to override the following:        */
/* the state array                                         */
/* the two previous outputs                                */
/* the current output                                      */
/* the three states for the taus algorithm                 */
/*                                                         */
/* See etausstrt.c for an initialization routine based     */
/* on a seed parameter.                                    */
/*                                                         */
/* The period length of etaus is approximates 1.2e+61036.  */
/* The period length of GSL taus2 is 3.09e+26.             */
/* The speed of etaus is 1/6 faster than the GSL version   */
/* of taus2.                                               */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include "etaus.h"

/* size of the state array is 2^14 */
#define STATES (16384)

etfmt *etausinit(void)
   {
   int i;                      /* loop counter */
   unsigned int dttk;          /* combined date, time, and #ticks */
   unsigned int *stp,*stq;     /* pointer into state array */
   time_t now;                 /* current date and time */
   clock_t clk;                /* current number of ticks */
   struct tms t;               /* structure used by times() */
   etfmt *et;                  /* etaus structure */

   /***************************************************/
   /* allocate memory for etaus structure */
   /***************************************************/
   et = (etfmt *) malloc(sizeof(etfmt));
   if (et == NULL)
      {
      fprintf(stderr,"etausinit: out of memory "
      "allocating etaus structure et\n");
      exit(1);
      } /* out of memory */
   et->states = STATES; /* save the number of etaus states */

   /***************************************************/
   /* allocate memory for etaus state array           */
   /***************************************************/
   et->state = (unsigned int *)
      malloc(sizeof(unsigned int) * STATES);
   if (et->state == NULL)
      {
      fprintf(stderr,"etausinit: out of memory "
      "allocating et->state state array\n");
      exit(1);
      } /* out of memory */

   /***************************************************/
   /* Randomize the seeds and states                  */
   /***************************************************/
   /* get clock ticks since boot                           */
   clk = times(&t);
   /* get date & time                                      */
   time(&now);
   /* combine date, time, and ticks into a single UINT     */
   dttk = (unsigned int) (now ^ clk);
   /* initialize the three taus states to date,time,ticks  */
   et->s1 = now;      /* initialize s1 to date, time       */
   et->s2 = clk;      /* initialize s2 to ticks            */
   et->s3 = dttk;     /* initialize s3 to combined dttk    */

   /***************************************************/
   /* Warm up the taus states.                        */
   /***************************************************/
   i = 128;
   while (i--) et->out = TAUS;

   /***************************************************/
   /* initialize the state array to random values     */
   /***************************************************/
   stp = (unsigned int *) et->state;
   stq = (unsigned int *) et->state + STATES;
   while (stp < stq)
      {
      *stp++ = TAUS;            /* set to random UINT */
      } /* for each member in et->state array */

   /***************************************************/
   /* initialize out, prev, and prev prev             */
   /* to random values                                */
   /***************************************************/
   et->pprev = TAUS;            /* set to random UINT */
   et->prev  = TAUS;            /* set to random UINT */
   et->out   = TAUS;            /* set to random UINT */

   /***************************************************/
   /* after this subroutine you may initialize the    */
   /* state array to your own values, if you wish     */
   /* to do regression testing.                       */
   /* Use the above 12 instructions as an example of  */
   /* how to initialize the generator                 */
   /* Use etausstrt(s1,s2,s3) to initialize with      */
   /* three seeds.                                    */
   /***************************************************/
   /* return the etaus structure                      */
   /***************************************************/
   return(et);
   } /* etausinit subroutine */
