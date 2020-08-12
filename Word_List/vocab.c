#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define WRDLEN (64)

typedef struct treestruct {
   struct treestruct *left;
   struct treestruct *rght;
   char word[WRDLEN];
   int count;
   } treefmt;

int cmpr(unsigned char *a, unsigned char *b)
   {
   int rslt;
   unsigned char *p,*q,*r;
   rslt = 0;
   p = (unsigned char *) a;
   q = (unsigned char *) a + WRDLEN;
   r = (unsigned char *) b;
   while (p < q)
      {
      rslt = *r++ - *p++;
      if (rslt) break;
      } /* for each char in word */
   return(rslt);
   } /* cmpr */

void cpy(unsigned char *a, unsigned char *b)
   {
   unsigned char *p,*q,*r;
   p = (unsigned char *) a;
   q = (unsigned char *) a + WRDLEN;
   r = (unsigned char *) b;
   while (p < q)
      {
      *p = *r;
      if (*p == '\0') break;
      p++;
      r++;
      } /* for each char in word */
   } /* cpy */

treefmt *inittree(void)
   {
   char *p,*q;
   treefmt *newnode;
   newnode = (treefmt *) malloc(sizeof(treefmt));
   if (newnode == NULL)
      {
      fprintf(stderr,"inittree: out of memory "
         "allocating newnode\n");
      exit(1);
      } /* out of memory */
   newnode->left = NULL;
   newnode->rght = NULL;
   p = (char *) newnode->word;
   q = (char *) newnode->word + WRDLEN;
   while (p < q) *p++ = '\0';
   newnode->count = 0;
   return(newnode);
   } /* inittree */

/* re-entrant routine */
void isrt(char *word, treefmt *node)
   {
   int rslt;
   treefmt *newnode;
   rslt = strcmp(word,node->word);
   // printf("isrt: rslt %d word %s nodeword %s\n",
      // rslt, word, node->word);
   if (rslt < 0)
      {
      if (node->left != NULL) isrt(word,node->left);
      else
         {
	 newnode = inittree();
	 strcpy(newnode->word,word);
	 newnode->count = 1;
	 node->left = (treefmt *) newnode;
	 } /* else less_than leaf node */
      } /* if less than */
   else if (rslt > 0)
      {
      if (node->rght != NULL) isrt(word,node->rght);
      else
         {
	 newnode = inittree();
	 strcpy(newnode->word,word);
	 newnode->count = 1;
	 node->rght = (treefmt *) newnode;
	 } /* else greater than leaf node */
      } /* if greater than */
   else    /* duplicate key */
      {
      node->count++;
      } /* if equal */
   } /* isrt */

/* re-entrant routine */
void rmtree(treefmt *node)
   {
   if (node->left != NULL) rmtree(node->left);
   if (node->rght != NULL) rmtree(node->rght);
   free(node);
   } /* rmtree */

/* re-entrant routine */
void traverse(treefmt *node)
   {
   // printf("traverse: %d %s\n",
      // node->count, node->word);
   if (node->left != NULL) traverse(node->left);
   printf("%09d %s\n", node->count, node->word);
   if (node->rght != NULL) traverse(node->rght);
   } /* traverse */

int getbyte(void)
   {
   int len;
   int quote;
   char buf[8];
   len = read(0,buf,1);
   if (!len) return(EOF);
   if (len != 1)
      {
      fprintf(stderr,"getbyte: read error\n");
      exit(1);
      } /* read error */
   /* force 7 bit alpha numeric text */
   /* 0x27 is a single quote in ASCII */
   if (buf[0] == 0x27) quote = 0x27;
   else quote = 0;
   if (buf[0] > 'z') buf[0] = ' ';
   else if (buf[0] < '0') buf[0] = ' ';
   else if (buf[0] > '9' && buf[0] < 'A') buf[0] = ' ';
   else if (buf[0] > 'Z' && buf[0] < 'a') buf[0] = ' ';
   if (quote == 0x27) buf[0] = 0x27;
   return(buf[0]);
   } /* getbyte */

int main(void)
   {
   char word[128];
   treefmt *root;
   root = (treefmt *) inittree();
   while (1)
      {
      int ch;
      int len;
      char *p,*q;
      len = 0;
      p = (char *) word;
      q = (char *) word + 63;
      while (p < q)
         {
         ch = getbyte();
         if (ch == EOF) break;
	 if (ch >= 'a' && ch <= 'z') ch -= 32;
	 if (ch == ' ') break;
	 *p++ = (char) ch;
	 len++;
         } /* for each char in word */
      *p = '\0';
      if (len) isrt(word,root);
      if (ch == EOF) break;
      } /* for each character in text */
   if (root->rght != NULL) traverse(root->rght);
   if (root->rght != NULL) rmtree(root->rght);
   free(root);
   return(0);
   } /* main */
