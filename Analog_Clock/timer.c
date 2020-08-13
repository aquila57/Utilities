/*
timer.c Version 0.1.0. Analog clock timer
Copyright (C) 2020 aquila57  at github.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to:

	Free Software Foundation, Inc.
	59 Temple Place - Suite 330
	Boston, MA  02111-1307, USA.
*/

#define FULLCIRCLE (360*64)

/* to define the escape key */
#define XK_MISCELLANY 1
#define XK_LATIN1 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/times.h>
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <assert.h>
#include <math.h>

#define NIL (0)

typedef struct xxstruct {
   int whiteColor,blackColor;
   int redColor;
   int rootwh;      /* root window height */
   int rootww;      /* root window width  */
   int dpyhght;     /* display window height */
   int dpywdth;     /* display window width  */
   int repeat;
   int runflg;
   int winticks;
   Display *dpy;
   Window w;
   GC gc;
   Font fontid;
   Screen *dfltscr;
   Colormap cmap;
   XColor scrdef,exctdef;
   double pi;
   double twopi;
   int fullcrcl;
   int xc;
   int yc;
   int arr;
   int minarr;
   int hrarr;
   int erase;      /* 0 = no, 1 = yes */
   int hh;
   int mm;
   int ss;
   int totsecs;
   time_t t;
   time_t timezero;
   time_t timestop;
   struct tm *tm;
   } xxfmt;

void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s seconds\n", pgm);
   fprintf(stderr,"Example: %s 300\n", pgm);
   fprintf(stderr,"This example sets the "
      "timer to 5 minutes\n");
   exit(1);
   } /* putstx */

/* X Windows code is based on: */
/* http://tronche.lri.fr:8000/gui/x/xlib-tutorial/2nd-program-anatomy.html */

void getkey(xxfmt *xx)
   {
   /* after each race: wait for command or exposure */

   XSelectInput(xx->dpy, xx->w,
      KeyPressMask|ExposureMask);

   while(1)
      {
      int symbol;
      XEvent e;
      XKeyEvent *k;
      XNextEvent(xx->dpy, &e);
      if (e.type == KeyPress)
         {
         k = (XKeyEvent *) &e;
         symbol = XLookupKeysym(k,0);
         xx->repeat = 0;
         if (symbol == XK_Escape || symbol == 'q')
            {
            xx->repeat = 0;
            xx->runflg = 0;
            break;
            } /* if quit */
         else if (symbol == 'r' || symbol == 'n')
            {
            xx->repeat = 1;
            break;
            } /* if next */
         } /* if keypress event */
      else if (e.type == Expose)
         {
         continue;
         } /* if expose event */
      } /* wait for window shutdown */
   } /* getkey */

/* test keyboard for interrupt event */

void ifkey(xxfmt *xx)
   {
   int msk;
   int symbol;
   int XCheckMaskEvent();
   XEvent e;
   XKeyEvent *k;

   msk = KeyPressMask|ExposureMask;

   XSelectInput(xx->dpy, xx->w, msk);

   while (XCheckMaskEvent(xx->dpy, msk, &e))
      {
      if (e.type == KeyPress)
         {
         k = (XKeyEvent *) &e;
         symbol = XLookupKeysym(k,0);
         if (symbol == XK_Escape
	    || symbol == XK_q)
            {
            xx->runflg = 0;
            } /* if quit */
         } /* if keypress event */
      else if (e.type == Expose)
         {
         } /* if expose event */
      } /* if event received */
   } /* ifkey */

/* Start new X Windows session */
/* Create X Window */

void initx(xxfmt *xx)
   {
   int rslt;
   char title[64];

   xx->dpy = XOpenDisplay(NIL);

   if (xx->dpy == NULL)
      {
      fprintf(stderr,"X Windows failure\n");
      exit(1);
      } /* if X Windows is not active */

   assert(xx->dpy);

   /* get dimensions of root window */
   xx->rootwh = XDisplayWidth(xx->dpy,0);
   xx->rootwh = XDisplayHeight(xx->dpy,0);

   /* make display window smaller than root window */
   /* allow for menu bar on top */
   xx->dpywdth = xx->rootwh - 100;
   xx->dpyhght = xx->rootwh - 100;

   xx->whiteColor = WhitePixel(xx->dpy, DefaultScreen(xx->dpy));
   xx->blackColor = BlackPixel(xx->dpy, DefaultScreen(xx->dpy));

   xx->w = XCreateSimpleWindow(xx->dpy,
      DefaultRootWindow(xx->dpy),
      0, 0, 
      xx->dpywdth, xx->dpyhght,
      0, xx->whiteColor,
      xx->whiteColor);

   XSelectInput(xx->dpy, xx->w, StructureNotifyMask);

   XMapWindow(xx->dpy, xx->w);

   xx->gc = XCreateGC(xx->dpy, xx->w, 0, NIL);

   xx->fontid = (Font) XLoadFont(xx->dpy,"12x24");

   XSetFont(xx->dpy,xx->gc,xx->fontid);

   XSetForeground(xx->dpy, xx->gc, xx->blackColor);

   xx->dfltscr = XDefaultScreenOfDisplay(xx->dpy);
   if (xx->dfltscr == NULL)
      {
      fprintf(stderr,"XDefaultScreenOfDisplay failed\n");
      perror("XDefaultScreenOfDisplay failed");
      exit(1);
      } /* if error */

   xx->cmap = XDefaultColormapOfScreen(xx->dfltscr);

   rslt = XAllocNamedColor(xx->dpy,xx->cmap,"red",
      &xx->scrdef,&xx->exctdef);

   if (rslt < 0)
      {
      fprintf(stderr,"XAllocNamedColor failed\n");
      perror("XAllocNamedColor failed");
      exit(1);
      } /* if error */
   xx->redColor = xx->scrdef.pixel;

   XSetWindowBorderWidth(xx->dpy, xx->w, 40);

   sprintf(title,"Clock");
   XStoreName(xx->dpy,xx->w,title);
   XSetIconName(xx->dpy,xx->w,title);

   while(1)
      {
      XEvent e;
      XNextEvent(xx->dpy, &e);
      if (e.type == MapNotify) break;
      } /* wait for window initialization */

   } /* initx */

void gettim(xxfmt *xx)
   {
   time_t timeleft;
   char str[256];
   time(&xx->t);
   timeleft = xx->timestop - xx->t;
   timeleft += 3600 * 6;
   xx->tm = localtime(&timeleft);
   xx->hh = xx->tm->tm_hour;
   xx->mm = xx->tm->tm_min;
   xx->ss = xx->tm->tm_sec;
   sprintf(str,"%02d:%02d:%02d",
      xx->hh, xx->mm, xx->ss);
   XStoreName(xx->dpy,xx->w,str);
   XSetIconName(xx->dpy,xx->w,str);
   } /* gettim */

/* draw one red pixel on the screen */

void putpxl(xxfmt *xx, int x, int y)
   {
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->redColor);
   XDrawPoint(xx->dpy,xx->w,xx->gc,x,y);
   } /* putpxl */

/* Bresenham's algorithm   */
/* Function to put pixels  */
/* at subsequence points   */

void drawCircle(xxfmt *xx, int x, int y) 
   { 
   putpxl(xx, xx->xc+x, xx->yc+y); 
   putpxl(xx, xx->xc-x, xx->yc+y); 
   putpxl(xx, xx->xc+x, xx->yc-y); 
   putpxl(xx, xx->xc-x, xx->yc-y); 
   putpxl(xx, xx->xc+y, xx->yc+x); 
   putpxl(xx, xx->xc-y, xx->yc+x); 
   putpxl(xx, xx->xc+y, xx->yc-x); 
   putpxl(xx, xx->xc-y, xx->yc-x); 
   } 
  
/* Function for circle-generation   */
/* using Bresenham's algorithm      */

void circleBres(xxfmt *xx)
   { 
   int x = 0, y = xx->arr; 
   int d = 3 - 2 * xx->arr; 
   drawCircle(xx, x, y); 
   while (y >= x) 
      { 
      /**********************************/
      /* for each pixel we will         */
      /* draw all eight pixels          */
      /**********************************/
      x++; 
      /**********************************/
      /* check for decision parameter   */
      /* and correspondingly            */
      /* update d, x, y                 */
      /**********************************/
      if (d > 0) 
         { 
         y--;  
         d = d + 4 * (x - y) + 10; 
         } 
      else
         d = d + 4 * x + 6; 
      drawCircle(xx, x, y); 
      } /* while x >= y */
   } /* circleBres */

/* calculate absolute value of an integer */

int abs(int x)
   {
   if (x < 0) return(-x);
   return(x);
   } /* abs */

/* erase one pixel on the screen */

void unplot(xxfmt *xx, int x, int y)
   {
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->whiteColor);
   XDrawPoint(xx->dpy,xx->w,xx->gc,x,y);
   } /* unplot */

/* draw one red pixel on the screen */

void plot(xxfmt *xx, int x, int y)
   {
   if (xx->erase)
      {
      unplot(xx, x, y);
      } /* if erase the line */
   else
      {
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->redColor);
      XDrawPoint(xx->dpy,xx->w,xx->gc,x,y);
      } /* else print the line */
   } /* plot */

/* draw a straight line using Bresenham's algorithm */

void plotLineLow(xxfmt *xx, int x0,int y0, int x1,int y1)
   {
   int dx,dy;
   int yi;
   int D;
   int x,y;
   dx = x1 - x0;
   dy = y1 - y0;
   yi = 1;
   if (dy < 0)
      {
      yi = -1;
      dy = -dy;
      } /* if dy < 0 */
   D = 2*dy - dx;
   y = y0;
   x = x0;
   while (x <= x1)
      {
      plot(xx, x,y);
      if (D > 0)
         {
         y += yi;
         D = D - 2*dx;
         } /* if D > 0 */
      D = D + 2*dy;
      x++;
      } /* while x <= x1 */
   } /* plotLineLow */

/* draw a straight line using Bresenham's algorithm */

void plotLineHigh(xxfmt *xx, int x0, int y0, int x1, int y1)
   {
   int dx,dy;
   int xi;
   int x,y;
   int D;
   dx = x1 - x0;
   dy = y1 - y0;
   xi = 1;
   if (dx < 0)
      {
      xi = -1;
      dx = -dx;
      } /* if dx < 0 */
   D = 2*dx - dy;
   x = x0;
   y = y0;
   while (y <= y1)
      {
      plot(xx, x,y);
      if (D > 0)
         {
         x = x + xi;
         D = D - 2*dy;
         } /* if D > 0 */
      D = D + 2*dx;
      y++;
      } /* while y <= y1 */
   } /* plotLineHigh */

/* draw a straight line using Bresenham's algorithm */

void plotLine(xxfmt *xx, int x0, int y0, int x1, int y1)
   {
   if (abs(y1 - y0) < abs(x1 - x0))
      {
      if (x0 > x1)
         plotLineLow(xx, x1, y1, x0, y0);
      else
         plotLineLow(xx, x0, y0, x1, y1);
      } /* if abs(y1-y0) < abs(x1-x0) */
   else
      {
      if (y0 > y1)
         plotLineHigh(xx, x1, y1, x0, y0);
      else
         plotLineHigh(xx, x0, y0, x1, y1);
      } /* else abs(y1-y0) >= abs(x1-x0) */
   } /* plotLine */

/* erase a straight line using Bresenham's algorithm */

void eraseLine(xxfmt *xx, int x0, int y0, int x1, int y1)
   {
   xx->erase = 1;
   plotLine(xx, x0, y0, x1, y1);
   xx->erase = 0;
   } /* eraseLine */

/* draw the tick marks on the clock face */

void putticks(xxfmt *xx)
   {
   int min;
   int exx;
   int wyy;
   int hrexx;
   int hrwyy;
   int minexx;
   int minwyy;
   double dblmin;
   double dblarr;
   double dblminarr;
   double dblhrarr;
   double thetarad;
   double thetadeg;
   double sinx;
   double cosx;
   double dblexx;
   double dblwyy;
   dblarr = (double) xx->arr;
   dblhrarr = (double) xx->arr * 0.92;
   dblminarr = (double) xx->arr * 0.96;
   min = 0;
   while (min < 60)
      {
      dblmin = (double) min;
      thetadeg = ((dblmin * 6.0) + 270.0);
      if (thetadeg >= 360.0) thetadeg -= 360.0;
      thetarad = (thetadeg / 360.0) * xx->twopi;
      if (thetarad < 0.0) thetarad += xx->twopi;
      sinx = sin(thetarad);
      cosx = cos(thetarad);
      /********************************************************/
      dblexx = cosx * dblarr; 
      dblwyy = sinx * dblarr; 
      exx = (int) dblexx;
      wyy = (int) dblwyy;
      exx += xx->xc;
      wyy += xx->yc;
      /********************************************************/
      if (min%5 == 0)
         {
         dblexx = cosx * dblhrarr; 
         dblwyy = sinx * dblhrarr; 
         hrexx  = (int) dblexx;
         hrwyy  = (int) dblwyy;
         hrexx += xx->xc;
         hrwyy += xx->yc;
         plotLine(xx, hrexx, hrwyy, exx, wyy);
         } /* if every 5 minutes */
      else
         {
         dblexx = cosx * dblminarr; 
         dblwyy = sinx * dblminarr; 
         minexx = (int) dblexx;
         minwyy = (int) dblwyy;
         minexx += xx->xc;
         minwyy += xx->yc;
         plotLine(xx, minexx, minwyy, exx, wyy);
         } /* else not hour tick */
      /********************************************************/
      min++;
      } /* for each minute tick */
  } /* putticks */

/* write the 12 hour numbers on the clock face */

void puthrs(xxfmt *xx)
   {
   XDrawString(xx->dpy, xx->w, xx->gc,
	   338, 102, "12", 2);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   475, 132, "1", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   570, 233, "2", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   602, 360, "3", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   570, 493, "4", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   475, 583, "5", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   345, 618, "6", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   215, 585, "7", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   120, 490, "8", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   85, 360, "9", 1);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   120, 230, "10", 2);
   XDrawString(xx->dpy, xx->w, xx->gc,
	   210, 140, "11", 2);
   } /* puthrs */

/* second loop to tell time on the clock */

void telltime(xxfmt *xx)
   {
   int hrexx;
   int hrwyy;
   int minexx;
   int minwyy;
   int secexx;
   int secwyy;
   double dblarr;
   double dblminarr;
   double dblhrarr;
   double dblhh;
   double dblmm;
   double dblss;
   double thetarad;
   double thetadeg;
   double sinx;
   double cosx;
   double dblexx;
   double dblwyy;
   dblarr = (double) xx->arr;
   dblminarr = (double) xx->minarr;
   dblhrarr = (double) xx->hrarr;
   while (xx->runflg && xx->t < xx->timestop)
      {
      circleBres(xx);    /* function call  */
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->blackColor);
      XFillArc(xx->dpy, xx->w, xx->gc,
         xx->xc-10, xx->yc-10,
         20, 20, 0, xx->fullcrcl);
      puthrs(xx);
      XSetForeground(xx->dpy, xx->gc, xx->redColor);
      putticks(xx);
      gettim(xx);
      dblhh = (double) xx->hh;
      dblmm = (double) xx->mm;
      dblss = (double) xx->ss;
      /*******************************************************/
      /* plot the second hand                                */
      /*******************************************************/
      thetadeg = ((dblss * 6.0) + 270.0);
      if (thetadeg >= 360.0) thetadeg -= 360.0;
      thetarad = (thetadeg / 360.0) * xx->twopi;
      if (thetarad < 0.0) thetarad += xx->twopi;
      sinx = sin(thetarad);
      cosx = cos(thetarad);
      dblexx = cosx * dblarr; 
      dblwyy = sinx * dblarr; 
      secexx = (int) dblexx;
      secwyy = (int) dblwyy;
      secexx += xx->xc;
      secwyy += xx->yc;
      plotLine(xx, xx->xc, xx->yc, secexx, secwyy);
      /*******************************************************/
      /* plot the minute hand                                */
      /*******************************************************/
      thetadeg = ((dblmm * 6.0) + 270.0);
      if (thetadeg >= 360.0) thetadeg -= 360.0;
      thetarad = (thetadeg / 360.0) * xx->twopi;
      if (thetarad < 0.0) thetarad += xx->twopi;
      sinx = sin(thetarad);
      cosx = cos(thetarad);
      dblexx = cosx * dblminarr; 
      dblwyy = sinx * dblminarr; 
      minexx = (int) dblexx;
      minwyy = (int) dblwyy;
      minexx += xx->xc;
      minwyy += xx->yc;
      plotLine(xx, xx->xc, xx->yc, minexx, minwyy);
      /*******************************************************/
      /* plot the hour hand                                  */
      /*******************************************************/
      thetadeg = ((dblhh * 30.0) + 270.0);
      if (thetadeg >= 360.0) thetadeg -= 360.0;
      thetarad = (thetadeg / 360.0) * xx->twopi;
      if (thetarad < 0.0) thetarad += xx->twopi;
      sinx = sin(thetarad);
      cosx = cos(thetarad);
      dblexx = cosx * dblhrarr; 
      dblwyy = sinx * dblhrarr; 
      hrexx = (int) dblexx;
      hrwyy = (int) dblwyy;
      hrexx += xx->xc;
      hrwyy += xx->yc;
      plotLine(xx, xx->xc, xx->yc, hrexx, hrwyy);
      /*******************************************************/
      /* flush the new time                                  */
      /*******************************************************/
      XFlush(xx->dpy);
      sleep(1);
      /*******************************************************/
      /* eraase the second hand                              */
      /*******************************************************/
      eraseLine(xx, xx->xc, xx->yc, secexx, secwyy);
      /*******************************************************/
      /* eraase the minute hand                              */
      /*******************************************************/
      eraseLine(xx, xx->xc, xx->yc, minexx, minwyy);
      /*******************************************************/
      /* eraase the hour hand                                */
      /*******************************************************/
      eraseLine(xx, xx->xc, xx->yc, hrexx, hrwyy);
      /*******************************************************/
      /* flush the old time                                  */
      /*******************************************************/
      XFlush(xx->dpy);
      ifkey(xx);
      } /* infinite second loop */
   } /* telltime */

/* flash the screen to indicate timer ended */

void done(xxfmt *xx)
   {
   int i;
   i = 1000;
   while (xx->runflg && i--)
      {
      int len;
      char str[256];
      sprintf(str,"Timer is finished");
      len = strlen(str);
      XClearWindow(xx->dpy, xx->w);
      XClearArea(xx->dpy, xx->w,
         0, 0, xx->dpywdth, xx->dpyhght, 0);
      XSetBackground(xx->dpy, xx->gc, xx->blackColor);
      XSetForeground(xx->dpy, xx->gc, xx->blackColor);
      XFillRectangle(xx->dpy, xx->w, xx->gc,
         0, 0, xx->dpywdth, xx->dpyhght);
      XSetForeground(xx->dpy, xx->gc, xx->whiteColor);
      XDrawString(xx->dpy, xx->w, xx->gc,
         xx->xc - 100, xx->yc, str, len);
      XFlush(xx->dpy);
      sleep(1);
      XClearWindow(xx->dpy, xx->w);
      XClearArea(xx->dpy, xx->w,
         0, 0, xx->dpywdth, xx->dpyhght, 0);
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->whiteColor);
      XFillRectangle(xx->dpy, xx->w, xx->gc,
         0, 0, xx->dpywdth, xx->dpyhght);
      XSetForeground(xx->dpy, xx->gc, xx->blackColor);
      XDrawString(xx->dpy, xx->w, xx->gc,
         xx->xc - 100, xx->yc, str, len);
      XFlush(xx->dpy);
      sleep(1);
      ifkey(xx);
      } /* loop 1000 seconds */
   } /* done */
  
int main(int argc, char **argv) 
   { 
   xxfmt *xx;
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of memory */
   if (argc != 2) putstx(*argv);
   xx->totsecs = atoi(*(argv+1));
   if (xx->totsecs < 1)
      {
      fprintf(stderr,"main: too few seconds\n");
      putstx(*argv);
      } /* too few seconds */
   if (xx->totsecs > 1000000)
      {
      fprintf(stderr,"main: too many seconds\n");
      putstx(*argv);
      } /* too many seconds */
   initx(xx);
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->blackColor);
   xx->fullcrcl = FULLCIRCLE;
   xx->arr    = ((xx->dpyhght-100) >> 1);
   xx->minarr = xx->arr * 0.9;
   xx->hrarr  = xx->arr * 0.65;
   xx->xc  = xx->yc = xx->arr + 50;
   xx->erase = 0;
   xx->runflg = xx->repeat = 1;
   xx->pi = M_PI;
   xx->twopi = 2.0 * xx->pi;
   gettim(xx);
   xx->timezero = xx->t;
   xx->timestop = xx->t + xx->totsecs;
   telltime(xx);
   done(xx);
   XDestroyWindow(xx->dpy,xx->w);
   XCloseDisplay(xx->dpy);
   free(xx);
   return(0); 
   } 
