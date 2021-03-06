/* hxline.c - Hexadecimal dump of 16 bytes  Version 0.1.0 */
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

/* print a 16 byte block in hex and ASCII */
int hxline(void)
   {
   int i;
   int len;
   unsigned char *p,*q;
   unsigned char str[32];
   int getbyte(void);
   /*******************************************/
   /* initialize input buffer to spaces       */
   /*******************************************/
   p = (unsigned char *) str;
   q = (unsigned char *) str + 16;
   while (p < q) *p++ = ' ';
   /*******************************************/
   /* read 16 bytes to input buffer           */ 
   /*******************************************/
   len = 0;
   p = (unsigned char *) str;
   q = (unsigned char *) str + 16;
   while (p < q)
      {
      int ch;
      ch = getbyte();
      if (ch == EOF) break;
      *p++ = (unsigned char) ch;
      len++;
      } /* for each byte on line */
   /*******************************************/
   /* print buffer in hex                     */
   /*******************************************/
   i = 0;
   p = (unsigned char *) str;
   q = (unsigned char *) str + len;
   while (p < q)
      {
      printf("%02x", *p++);
      i++;
      if (i == 4 || i == 12) printf(" ");
      else if (i == 8) printf("  ");
      } /* for each byte on line */
   /*******************************************/
   /* if end of file print trailing spaces    */
   /*******************************************/
   if (len < 16)
      {
      int j;
      j = 16 - len;
      while (j--)
         {
	 printf("  ");
         i++;
         if (i == 4 || i == 12) printf(" ");
         else if (i == 8) printf("  ");
	 } /* for each byte after end of file */
      } /* if end of input file */
   /*******************************************/
   /* print buffer in ASCII                   */
   /*******************************************/
   printf("  *");
   p = (unsigned char *) str;
   q = (unsigned char *) str + len;
   while (p < q)
      {
      if (*p < ' ' || *p > 0x7e) printf(" ");
      else printf("%c", *p);
      p++;
      } /* for each byte on line */
   /*******************************************/
   /* print trailing spaces at end of file    */
   /*******************************************/
   if (len < 16)
      {
      int j;
      j = 16 - len;
      while (j--) printf(" ");
      } /* if end of file */
   /*******************************************/
   /* print final asterisk on line            */
   /*******************************************/
   printf("*\n");
   /*******************************************/
   /* return                                  */
   /* 0 = not end of file                     */
   /* not 0 = end of file                     */
   /*******************************************/
   return(16 - len);
   } /* hxline */
