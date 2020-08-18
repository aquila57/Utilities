/* etausunif.c - etaus uniform random number 0-1 Version 2.0.0       */
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

/* this subroutine produces a uniform random number       */
/* from zero to one                                       */
/* For a slower method with higher precision, use         */
/* etausfrac() instead.                                   */

#define MAXINT 4294967296.0

#include "etaus.h"

double etausunif(etfmt *et)
   {
   double num;             /* uniform random number from 0-1  */
   num = (double) etaus(et) / MAXINT;
   return(num);            /* return uniform random number    */
   } /* etausunif */
