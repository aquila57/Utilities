/* hxmem.c - Hexadecimal dump subroutine  Version 0.1.0 */
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEN (16)

/* print a 16 byte block in hex and ASCII */
void hxline(unsigned char *mem)
   {
   int i;
   unsigned char *p,*q,*r;
   unsigned char str[32];
   /*******************************************/
   /* initialize input buffer to spaces       */
   /*******************************************/
   p = (unsigned char *) str;
   q = (unsigned char *) str + 16;
   r = (unsigned char *) mem;
   while (p < q) *p++ = *r++;
   /*******************************************/
   /* print buffer in hex                     */
   /*******************************************/
   i = 0;
   p = (unsigned char *) str;
   q = (unsigned char *) str + LEN;
   while (p < q)
      {
      printf("%02x", *p++);
      i++;
      if (i == 4 || i == 12) printf(" ");
      else if (i == 8) printf("  ");
      } /* for each byte on line */
   /*******************************************/
   /* print buffer in ASCII                   */
   /*******************************************/
   printf("  *");
   p = (unsigned char *) str;
   q = (unsigned char *) str + LEN;
   while (p < q)
      {
      if (*p < ' ' || *p > 0x7e) printf(" ");
      else printf("%c", *p);
      p++;
      } /* for each byte on line */
   /*******************************************/
   /* print final asterisk on line            */
   /*******************************************/
   printf("*\n");
   } /* hxline */

void hxmem(int len, unsigned char *mem)
   {
   int ofst;                     /* address of line */
   unsigned char *addr;          /* address of each line */
   unsigned char *memend;        /* end of block pointer */
   printf("%p\n", mem);
   ofst = 0;
   addr = (unsigned char *) mem;
   memend = (unsigned char *) mem + len;
   while (addr < memend)
      {
      if (ofst > 0xffffff)
	 {
	 fprintf(stderr,"hxmem: memory block "
	    "16 megabytes\n");
         exit(1);
	 } /* more than 16 meg */
      printf("%06x  ", ofst);    /* print line address */
      hxline(addr);         /* print 16 bytes in hex and ASCII */
      ofst += 16;
      addr += 16;
      } /* for each 16 byte line */
   } /* hxmem */
