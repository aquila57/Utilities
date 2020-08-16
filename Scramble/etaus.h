/* etaus.h - etaus random number generator header file Version 2.0.0 */
/* Copyright (C) 2020 aquila57 at github.com */

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
/* for many suggestions leading to the etaus RNG        */
/********************************************************/

/***************************************************************/
/* This version of etaus is based on:                          */
/* http://www.github.com/aquila62/etaus                        */
/* The size of the Bays-Durham shuffle has been increased.     */
/***************************************************************/

/* The subroutines below are called billions of times     */
/* in a single run.                                       */
/* Parameters to the subroutines must be validated by     */
/* the calling program, to maximize efficiency            */
/* and minimize redundant editing.                        */

typedef struct etausstruct {
   int states;               /* number of states = 16384 */
   unsigned int s1;          /* taus s1 */
   unsigned int s2;          /* taus s2 */
   unsigned int s3;          /* taus s3 */
   unsigned int ofst;        /* offset into state array */
   unsigned int out;         /* current output state */
   unsigned int prev;        /* prev output state */
   unsigned int pprev;       /* prev prev output state */
   unsigned int *state;      /* state array of 1024 members */
   } etfmt;

#define TAUSONE (et->s1 = (((et->s1&0xfffffffe)<<12) \
      ^(((et->s1<<13)^et->s1)>>19)))

#define TAUSTWO (et->s2 = (((et->s2&0xfffffff8)<< 4) \
      ^(((et->s2<< 2)^et->s2)>>25)))

#define TAUSTRE (et->s3 = (((et->s3&0xfffffff0)<<17) \
      ^(((et->s3<< 3)^et->s3)>>11)))

#define TAUS (TAUSONE ^ TAUSTWO ^ TAUSTRE)

etfmt *etausinit(void);                 /* initialization routine */
etfmt *etausstrt(unsigned int seed1, unsigned int seed2,
   unsigned int seed3);    /* initialization routine */
unsigned int etaus(etfmt *et);          /* random uint generator */
int etausbit(etfmt *et);                /* random bit generator */
double etausunif(etfmt *et);            /* random uniform number 0-1 */
double etausfrac(etfmt *et);            /* random uniform number 0-1 */
int etausint(etfmt *et, int limit);     /* random integer 0 up to limit */
unsigned int etauspwr(etfmt *et, int bits); /* random # 1-32 bits */
void etausdspl(etfmt *et, int option);    /* display etaus state */
void etausabt(void);                    /* etaus license summary */
void etausprm(unsigned int *lst);       /* prime number generator */
