#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void hxmem(int len, unsigned char *mem);

int main(void)
   {
   char txt[4096];
   char *heeptxt;
   heeptxt = (char *) malloc(4096);
   if (heeptxt == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating heeptxt\n");
      exit(1);
      } /* out of memory */
   printf("Dump of stack memory: ");
   strcpy(txt,"The quick brown fox jumps over the lazy dog.\n");
   hxmem(strlen(txt), (unsigned char *) txt);
   strcpy(heeptxt,"The quick brown fox jumps over the lazy dog.\n");
   printf("Dump of heep memory: ");
   hxmem(strlen(heeptxt), (unsigned char *) heeptxt);
   return(0);
   } /* main */
