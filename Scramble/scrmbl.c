/* scrmbl.c - Scramble a chess FENN file  Version 0.1.0              */
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

/************************************************************************/
/* This program reads a FENN file from stdin and scrambles the order    */
/* of the chess positions in the file.  Output is to stdout.            */
/*                                                                      */
/* Any list of text may be scrambled by this program.                   */
/* The default line_size is 512 bytes.                                  */
/* Change the LEN define, if you need a different line size.            */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "etaus.h"

/* define length of position line */

#define LEN (512)

/* define the structure of the binary sort tree */

typedef struct treestruct {
   struct treestruct *left;
   struct treestruct *rght;
   unsigned int key;                /* random key */
   char position[LEN];              /* one line of the FENN file */
   } treefmt;

/* initialize a new node in the tree */

treefmt *treeinit(void)
   {
   treefmt *newnode;
   char *p,*q;
   /* allocate a new tree node in the heap */
   newnode = (treefmt *) malloc(sizeof(treefmt));
   if (newnode == NULL)
      {
      fprintf(stderr,"treeinit: out of memory "
         "allocating newnode\n");
      exit(1);
      } /* out of memory */
   newnode->left = NULL;
   newnode->rght = NULL;
   newnode->key  = 0;
   p = (char *) newnode->position;
   q = (char *) newnode->position + LEN;
   while (p < q) *p++ = '\0';
   return(newnode);
   } /* treeinit */

/* insert a new chess position in the random sort tree */
/* duplicates are not allowed */
/* in case of duplicate error, re-run job */
/* duplicates should not occur */

void isrt(treefmt *node, unsigned int key, char *position)
   {
   treefmt *newnode;
   if (key < node->key)
      {
      if (node->left != NULL) isrt(node->left, key, position);
      else
         {
	 newnode = (treefmt *) treeinit();
	 newnode->key = key;
	 strcpy(newnode->position,position);
	 node->left = (treefmt *) newnode;
	 } /* leaf node: insert new node in tree */
      } /* if key is less than current node */
   else if (key > node->key)
      {
      if (node->rght != NULL) isrt(node->rght, key, position);
      else
         {
	 newnode = (treefmt *) treeinit();
	 newnode->key = key;
	 strcpy(newnode->position,position);
	 node->rght = (treefmt *) newnode;
	 } /* leaf node: insert new node in tree */
      } /* if key is greater than current node */
   else
      {
      fprintf(stderr,"isrt: duplicate key error\n");
      fprintf(stderr,"re-run job\n");
      exit(1);
      } /* duplicate key error */
   } /* isrt */

/* print chess positions in random sort tree */

void traverse(treefmt *node)
   {
   if (node->left != NULL) traverse(node->left);
   printf("%s\n", node->position);
   if (node->rght != NULL) traverse(node->rght);
   } /* traverse */

/* re-entrant routine to free all nodes in the sort tree */
/* nodes in the tree are freed bottom up */

void rmtree(treefmt *node)
   {
   if (node->left != NULL) rmtree(node->left);
   if (node->rght != NULL) rmtree(node->rght);
   free(node);
   } /* rmtree */

/* read one byte from stdin */

int getbyte(void)
   {
   int len;
   char buf[8];
   len = read(0,buf,1);
   if (!len) return(EOF);
   if (len != 1)
      {
      fprintf(stderr,"getbyte: read error\n");
      exit(1);
      } /* read error */
   return(buf[0]);
   } /* getbyte */

/* read one chess position from stdin */
/* returns the length of the line */
/* a line of zero length indicates end of file */

int getposition(char *position)
   {
   int len;
   char *p,*q;
   /* initialize chess position to zeros */
   p = (char *) position;
   q = (char *) position + LEN;
   while (p < q) *p++ = '\0';
   /* fill chess position one byte at a time from stdin */
   len = 0;
   p = (char *) position;
   q = (char *) position + LEN - 1;
   while (p < q)
      {
      int ch;
      ch = getbyte();              /* read one byte from stdin */
      if (ch == EOF) return(EOF);  /* bounce out if end of file */
      if (ch == '\r') continue;    /* ignore 0x0d */
      if (ch == '\n') break;       /* end of line is line feed */
      if (ch == '\t')              /* if tab character */
         {
	 len += 4;
	 *p++ = ' ';
	 *p++ = ' ';
	 *p++ = ' ';
	 *p++ = ' ';
	 continue;
	 } /* if tab character read */
      /* input must be ASCII characters */ 
      if (ch < ' ' || ch > '~')
         {
	 fprintf(stderr,"getposition: non ASCII input data\n");
	 fprintf(stderr,"character %02x at "
	    "position %d\n", ch, len+1);
	 exit(1);
	 } /* non ASCII error */
      len++;   /* tally the length of the line */
      *p++ = (char) ch;    /* fill the line one byte at a time */
      } /* for each character in chess position */
   /* quit if there is overflow */
   /* each chess position should be less than 80 characters */
   /* this error should never occur */
   if (p >= q)
      {
      *p = '\0';
      fprintf(stderr,"getposition: input data overflow\n");
      fprintf(stderr,"position: %s\n", position);
      exit(1);
      } /* overflow error */
   return(len);
   } /* getposition */

int main(void)
   {
   int len;           /* end of file indicator */
   unsigned int lst[8];     /* seeds for random number generator */
   char position[LEN];      /* data field for chess position */
   treefmt *root;           /* root node of sort tree */
   etfmt *et;               /* random number generator structure */
   /********************************************************************/
   /* calculate the random seeds for etausstrt                         */
   /********************************************************************/
   etausprm(lst);           /* generate three random seeds */
   fprintf(stderr,"etausstrt seeds:  %d  %d  %d\n",
      lst[0], lst[1], lst[2]);
   /********************************************************************/
   /* initialize random number generator                               */
   /********************************************************************/
   et = (etfmt *) etausstrt(lst[0], lst[1], lst[2]);
   /* create root node for sort tree */
   /* data for the tree is under the root node */
   root = treeinit();
   /* main loop is terminated by end of file */
   while (1)
      {
      unsigned int key;        /* random sort key */
      len = getposition(position);    /* read one chess position */
      if (len == EOF) break;          /* bounce out at end of file */
      key = etauspwr(et,30);        /* generate random 30 bit key */
      isrt(root,key,position);      /* insert chess position in tree */
      } /* for each FENN position */
   /* free random number generator structure from memory */
   free(et->state);
   free(et);
   /* at end of file, print the random sort tree */
   if (root->rght != NULL) traverse(root->rght);
   else fprintf(stderr,"main: empty tree\n");
   /* free tree nodes from memory */
   if (root->rght != NULL) rmtree(root->rght);
   free(root);
   return(0);
   } /* main */
