/* prmlst.c - Print one page of prime numbers  Version 0.1.0         */
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

/* This program prints one page of prime numbers equal or greater    */
/* than the input parameter.                                         */
/* Prime numbers are generated using the Sieve of Eratosthenes       */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LMT (1024*1024*16)

#define SPAN (2900)

/* this program generates a list of prime numbers using the */
/* Sieve of Eratosthenes */

void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s N\n", pgm);
   fprintf(stderr,"Where N is 3 to %d\n", LMT - SPAN - 1);
   fprintf(stderr,"Example: %s 60000\n", pgm);
   exit(1);
   } /* putstx */

/* shwlst is for debugging */

void shwlst(char *lst)
   {
   int i;
   char *p,*q;
   i = 0;
   p = (char *) lst;
   q = (char *) lst + LMT;
   while (p < q)
      {
      printf("%2x ", *p);
      i++;
      if (i >= 10)
         {
	 printf("\n");
	 i = 0;
	 } /* if end of line */
      p++;
      } /* for each offset */
   } /* shwlst */

int main(int argc, char **argv)
   {
   int i;
   int lmt;
   int strt;
   char *lst;
   char *p,*q;
   strt = LMT - SPAN;
   if (argc == 2) strt = atoi(*(argv+1)) | 1;
   if (strt < 3 || strt > LMT - SPAN)
      {
      fprintf(stderr,"%s:  parameter N "
         "out of range\n", *argv);
      putstx(*argv);
      } /* out of range */
   lmt = LMT;
   lst = (char *) malloc(lmt);
   if (lst == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating lst\n");
      exit(1);
      } /* out of memory */
   p = (char *) lst;
   q = (char *) lst + lmt;
   while (p < q) *p++ = '\0';
   p = (char *) lst + 4;
   q = (char *) lst + lmt;
   while (p < q)
      {
      *p++ = 0x01;
      p++;
      } /* for each even number */
   i = 3;
   while (i < LMT) 
      {
      p = (char *) lst + i;
      if (*p)
         {
	 i += 2;
	 continue;
	 } /* if already a multiple */
      q = (char *) lst + lmt;
      p += i;
      while (p < q)
         {
	 *p = 0x01;
	 p += i;
	 } /* for each multiple of i */
      i += 2;
      } /* for each multiple */
   if (strt == 3) printf("1 2 ");
   i = strt;
   p = (char *) lst + strt;
   q = (char *) lst + strt + SPAN;
   while (p < q)
      {
      if (!*p) printf("%d ", i);
      p += 2;
      i += 2;
      } /* for each offset in list */
   printf("\n");
   return(0);
   } /* main */
