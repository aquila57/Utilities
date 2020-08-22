/* crc16.c - 16 bit XMODEM-CRC subroutine  Version 0.1.0             */
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

/* This is the subroutine created by Chuck Forsberg */
/* for calculating the XMODEM-CRC */
/* This CRC is also called the CCITT CRC16 */
/* However there are several versions of CCITT CRC16 */
/* This subroutine agrees with the online CRC calculator */
/* https://crccalc.com for XMODEM-CRC */
/* This subroutine is a bit by bit CRC calculation */
/* It does not use a table in its calculation */
/* This subroutine is not a CRC update subroutine */
/* The CRC is always initialized to zero in this subroutine */
/* See crc16nxt.c for a CRC16 update subroutine. */
/* The CRC for 123456789 in this subroutine is 0x31C3. */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define POLY (0x1021)

unsigned int crc16(unsigned char *ptr, int len)
   {
   int i;
   unsigned int crc;
   unsigned char *p,*q;
   crc = 0;
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
   } /* crc16 */
