/* ascii.c - Print ASCII table  Version 0.1.0                        */
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

#include <stdio.h>

int main(void)
   {
   int i;           /* outer loop counter per row */
   int j;           /* inner loop counter per col */
   int k;           /* ascii character */
   printf("    0 1 2 3 4 5 6 7 "
      "8 9 a b c d e f\n");
   k = 0x20;
   i = 6;
   while (i--)
      {
      j = 16;
      printf("%02x  ", k);
      while (j--)
         {
	 if (k == 0x7f)
	    {
	    printf("  ");
	    k++;
	    } /* if k == 0x7f */
	 else printf("%c ", k++);
	 } /* for each column */
      printf("  %02x\n", k - 0x10);
      } /* for each row */
   printf("    0 1 2 3 4 5 6 7 "
      "8 9 a b c d e f\n");
   return(0);
   } /* main */
