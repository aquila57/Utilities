/* vocab.c - Word Count  Version 0.1.0                               */
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

/* This program counts the number of times each word occurs in */
/* a large text, such as a novel.                              */
/* The resulting word count is printed in alphabetic order.    */
/* The output file may be sorted in numeric sequence.          */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Define the maximum length of a word */

#define WRDLEN (64)

/* structure used in the binary sort tree */

typedef struct treestruct {
   struct treestruct *left;
   struct treestruct *rght;
   char word[WRDLEN];
   int count;
   } treefmt;

/* initialize a node in the binary sort tree */

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

/* insert a new node in the binary sort tree */
/* or increase the count of an already existing word */
/* in the tree. */

/* re-entrant routine */
void isrt(char *word, treefmt *node)
   {
   int rslt;
   treefmt *newnode;
   rslt = strcmp(word,node->word);
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
	 } /* else greater-than leaf node */
      } /* if greater than */
   else    /* duplicate key */
      {
      node->count++;     /* increase the word count by one */
      } /* if equal */
   } /* isrt */

/* free all nodes under the root of the tree */

/* re-entrant routine */
void rmtree(treefmt *node)
   {
   if (node->left != NULL) rmtree(node->left);
   if (node->rght != NULL) rmtree(node->rght);
   free(node);
   } /* rmtree */

/* traverse the tree in alphabetic order */
/* print the word count in alphabetic order */

/* re-entrant routine */
void traverse(treefmt *node)
   {
   // printf("traverse: %d %s\n",
      // node->count, node->word);
   if (node->left != NULL) traverse(node->left);
   printf("%09d %s\n", node->count, node->word);
   if (node->rght != NULL) traverse(node->rght);
   } /* traverse */

/* read one byte from stdin */
/* convert all special characters to space */
/* except for single quote */
/* return the new character or return EOF */

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
   char word[128];       /* storage for a single word */
   treefmt *root;        /* root of the binary sort tree */
   root = (treefmt *) inittree();    /* initialize the root */
   /* main loop is terminated by EOF */
   /* At EOF, the last word is added to the tree */
   while (1)     /* for each word in the text */
      {
      int ch;      /* stdin character read */
      int len;     /* length of a word */
      char *p,*q;     /* pointer to letters within a word */
      len = 0;        /* initialize word length */
      /* maximum word length is WRDLEN - 1 */
      p = (char *) word;
      q = (char *) word + WRDLEN - 1;
      while (p < q)
         {
	 /* read one character from stdin */
         ch = getbyte();
	 /* if end of file, terminate loop */
         if (ch == EOF) break;
	 /* convert lower case to upper case */
	 if (ch >= 'a' && ch <= 'z') ch -= 32;
	 /* terminate inner loop if delimiter read */
	 if (ch == ' ') break;
	 /* otherwise add new character to end of word */
	 *p++ = (char) ch;
	 len++;    /* increase word length by one */
         } /* for each char in word */
      *p = '\0';         /* end of string character */
      /* insert the new word in the tree */
      /* or increase its word count by one */
      if (len) isrt(word,root);
      /* terminate outer loop at end of file */
      if (ch == EOF) break;
      } /* for each word in text */
   /* the sort tree has been built */
   /* print the word list in alphabetic sequence */
   if (root->rght != NULL) traverse(root->rght);
   /* free all nodes in the tree under the root */
   if (root->rght != NULL) rmtree(root->rght);
   free(root);       /* free the tree root */
   return(0);
   } /* main */
