/*
xtimer.c Version 0.1.0. Analog clock timer
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
   fprintf(stderr,"Where seconds is 1-1000000\n");
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


/* erase a straight line using X Windows */

void eraseLine(xxfmt *xx, int x0, int y0, int x1, int y1)
   {
   xx->erase = 1;
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->whiteColor);
   XDrawLine(xx->dpy, xx->w, xx->gc,
      x0, y0, x1, y1);
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
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->redColor);
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
         XDrawLine(xx->dpy, xx->w, xx->gc,
            hrexx, hrwyy, exx, wyy);
         } /* if every 5 minutes */
      else
         {
         dblexx = cosx * dblminarr; 
         dblwyy = sinx * dblminarr; 
         minexx = (int) dblexx;
         minwyy = (int) dblwyy;
         minexx += xx->xc;
         minwyy += xx->yc;
         XDrawLine(xx->dpy, xx->w, xx->gc,
            minexx, minwyy, exx, wyy);
         } /* else not hour tick */
      /********************************************************/
      min++;
      } /* for each minute tick */
  } /* putticks */

/* write the 12 hour numbers on the clock face */

void puthrs(xxfmt *xx)
   {
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->blackColor);
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
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->redColor);
      /* x,y,wdth,hght,angle1,angle2 */
      XDrawArc(xx->dpy, xx->w, xx->gc,
	 xx->xc-xx->arr,xx->yc-xx->arr,
         xx->arr << 1, xx->arr << 1,
         0, xx->fullcrcl);
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
      XDrawLine(xx->dpy, xx->w, xx->gc,
         xx->xc, xx->yc, secexx, secwyy);
      /*******************************************************/
      /* plot the minute hand                                */
      /*******************************************************/
      thetadeg = ((dblmm * 6.0) + 270.0);
      thetadeg += (dblss / 60.0) * 6.0;
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
      XDrawLine(xx->dpy, xx->w, xx->gc,
         xx->xc, xx->yc, minexx, minwyy);
      /*******************************************************/
      /* plot the hour hand                                  */
      /*******************************************************/
      thetadeg = ((dblhh * 30.0) + 270.0);
      thetadeg += (dblmm / 12.0) * 6.0;
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
      XDrawLine(xx->dpy, xx->w, xx->gc,
         xx->xc, xx->yc, hrexx, hrwyy);
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
      system("beep -f 55 -l 200");
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
      system("beep -f 55 -l 200");
      sleep(1);
      ifkey(xx);
      } /* loop 1000 seconds */
   } /* done */
  
int main(int argc, char **argv) 
   { 
   int rslt;
   xxfmt *xx;
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of memory */
   if (argc != 2) putstx(*argv);
   rslt = strcmp(*(argv+1), "-h");
   if (!rslt) putstx(*argv);
   rslt = strcmp(*(argv+1), "--help");
   if (!rslt) putstx(*argv);
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
