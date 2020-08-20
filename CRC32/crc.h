/* crc.h - 32 bit Cyclical Reduncancy Check header  Version 0.1.0    */
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

/* This is the header file for a 32 bit cyclical redundancy check  */

/* https://www.cl.cam.ac.uk/research/srg/bluebook/21/crc/node6.html  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUOTIENT (0x04c11db7)

void bldtbl(unsigned int *tbl);
unsigned int crc32(unsigned char *str,
   int len, unsigned int *tbl);
unsigned int crc32nxt(unsigned int oldcrc,
   unsigned char *str, int len, unsigned int *tbl);
