/* crc16nxt.c - 16 bit XMODEM-CRC update subroutine  Version 0.1.0  */
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

/* This is a modified version of Chuck Forsberg's CCITT */
/* XMODEM-CRC subroutine. */
/* This is a CRC update subroutine.  */
/* The CCITT XMODEM-CRC is initialized to zero at the */
/* beginning.  This subroutine receives the CRC of */
/* the previous calculation. */
/* Therefore the first parameter must be zero for the */
/* first call.  */
/* This subroutine is a bit by bit CRC calculation */
/* It does not use a table in its calculation */
/* See subroutine crc16.c for an XMODEM-CRC calculation of a */
/* single block of data. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define POLY (0x1021)

/* This is an update subroutine. */
/* oldcrc is the CRC calculated from the previous call. */

unsigned int crc16nxt(unsigned int oldcrc,
   unsigned char *ptr, int len)
   {
   int i;
   unsigned int crc;
   unsigned char *p,*q;
   crc = oldcrc;
   p = (unsigned char *) ptr;
   q = (unsigned char *) ptr + len;
   while (p < q)
      {
      int j;
      j = *p++;
      crc = crc ^ (j << 8);
      i = 8;
      while (i--)
         {
	 if (crc & 0x8000)
	    {
	    crc = (crc << 1) ^ POLY;
	    } /* if crc high order bit == 1 */
	 else
	    {
	    crc = crc << 1;
	    } /* else crc high order bit == 0 */
	 } /* for each bit */
      } /* for each byte in ptr */
   return(crc & 0xffff);
   } /* crc16nxt */
