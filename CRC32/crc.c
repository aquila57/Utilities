/* crc.c - 32 bit Cyclical Reduncancy Check  Version 0.1.0           */
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

/* This is a 32 bit cyclical redundancy check  */
/* for a stdin input file */

/* The algorithm for crc32 comes from */
/* https://www.cl.cam.ac.uk/research/srg/bluebook/21/crc/node6.html  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* 64 MB buffer */
#define SIZE (65536 * 1024)

int getbyte(void)
   {
   int len;
   unsigned char buf[8];
   len = read(0,buf,1);
   if (!len) return(EOF);
   if (len != 1)
      {
      fprintf(stderr,"getbyte: read error\n");
      exit(1);
      } /* read error */
   return(buf[0]);
   } /* getbyte */

int fillbuf(unsigned char *buf)
   {
   int len;
   int ch;
   unsigned char *p,*q;
   p = (unsigned char *) buf;
   q = (unsigned char *) buf + SIZE;
   len = 0;
   while (p < q)
      {
      ch = getbyte();
      if (ch == EOF) break;
      *p++ = (unsigned char) ch;
      len++;
      } /* for each byte of input */
   return(len);
   } /* fillbuf */

int main(void)
   {
   int len;
   unsigned int crc;
   unsigned char *datta;
   unsigned int tbl[512];
   void bldtbl(unsigned int *tbl);
   unsigned int crc32(unsigned char *str, int len, unsigned int *tbl);
   datta = (unsigned char *) malloc(SIZE);
   if (datta == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating buffer datta\n");
      exit(1);
      } /* out of memory */
   bldtbl(tbl);
   len = fillbuf(datta);
   crc = crc32(datta,len,tbl);
   printf("crc %08x\n", crc);
   return(0);
   } /* main */
