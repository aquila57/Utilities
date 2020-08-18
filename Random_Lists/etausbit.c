/* etausbit.c - etaus random bit generator Version 2.0.0             */
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

/********************************************************/
/* Thanks to Dr. Robert G. Brown at Duke University     */
/* for many suggestions leading to this RNG.            */
/********************************************************/

/***************************************************************/
/* This version of etaus is based on:                          */
/* http://www.github.com/aquila62/etaus                        */
/* The size of the Bays-Durham shuffle has been increased.     */
/***************************************************************/

#include "etaus.h"

int etausbit(etfmt *et)
   {
   int i;
   int bit;
   unsigned int *p;           /* state pointer */
   unsigned int tmp;          /* used for bays-durham shuffle */
   /***********************************************************/
   /* back up the previous two states                         */
   /***********************************************************/
   et->ofst  = et->pprev >> 18;   /* offset into state array  */
   et->pprev = et->prev;   /* prev ==> prev prev              */
   et->prev  = et->out;    /* current ==> prev                */
   /***********************************************************/
   /* Calculate the taus algorithm inline to save a call      */
   /* to etaus()                                              */
   /***********************************************************/
   /* The TAUS macro is in etaus.h                            */
   /* XOR the previous two results with the current output    */
   et->out = (TAUS ^ et->pprev ^ et->prev);
   /***********************************************************/
   /* Bays-Durham shuffle of state array                      */
   /* 16384! = 1.2e+61036 base 10                             */
   /* The period length of the state array is very long       */
   /***********************************************************/
   /* point into the state array */
   p       = (unsigned int *) et->state + et->ofst;
   /* swap the current output with a member in the state array */
   tmp     = *p;
   *p      = et->out;
   et->out = tmp;
   /********************************************************/
   /* xor all 32 bits in et->out to get one output bit     */
   /* the output from etausbit() performs very well in     */
   /* dieharder -a and TestU01 Crush tests                 */
   /********************************************************/
   bit = 0;          /* initialize the output bit */
   tmp = et->out;    /* temporary copy of etaus output */
   i = 32;           /* loop counter for 32 bits */
   while (i--)       /* loop 32 times */
      {
      bit ^= tmp;    /* xor the low order bit to the output bit */
      tmp >>= 1;     /* shift to the next lower order bit */
      } /* for each bit in et->out */
   /* return output bit */
   return(bit & 1);
   } /* etausbit subroutine */
