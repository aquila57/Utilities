/*
xclk.c Version 0.1.0. Analog clock
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
   int xc;         /* x coordinate of a pixel */
   int yc;         /* y coordinate of a pixel */
   int arr;        /* radius of circle */
   int minarr;     /* length of minute hand */
   int hrarr;      /* length of hour hand */
   int erase;      /* 0 = no, 1 = yes */
   int hh;         /* 0-23 */
   int mm;         /* 0-59 */
   int ss;         /* 0-59 */
   int isdst;      /* 1=daylight saving time */
   int ampm;       /* 0=AM 1=PM */
   char ampmstr[8];    /* "AM" or "PM" */
   char dststr[8];     /* "Std" or "DST" */
   time_t t;           /* #seconds sinch the epoch */
   struct tm *tm;      /* time of day structure */
   } xxfmt;

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

/* Test keyboard for interrupt signal */

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

/* Initialize the XWindows server */
/* Create an X Window */

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

/* obtain the current time of day in seconds */

void gettim(xxfmt *xx)
   {
	char str[256];
	time(&xx->t);        /* obtain #seconds since the epoch */
	/* translates seconds to hours, minutes, seconds */
	xx->tm = localtime(&xx->t);
	if (xx->tm->tm_hour < 13)
	   {
	   xx->hh = xx->tm->tm_hour;
	   xx->ampm = 0;      /* set to AM */
		strcpy(xx->ampmstr,"AM");
	   } /* if AM */
	else
	   {
	   xx->hh = xx->tm->tm_hour - 12;
	   xx->ampm = 1;      /* set to PM */
		strcpy(xx->ampmstr,"PM");
	   } /* else PM */
	if (xx->tm->tm_hour == 12)
	   {
	   xx->ampm = 1;
		strcpy(xx->ampmstr,"PM");
	   } /* if 12 PM */
	xx->mm = xx->tm->tm_min;
	xx->ss = xx->tm->tm_sec;
	xx->isdst = xx->tm->tm_isdst;
        if (xx->isdst) strcpy(xx->dststr,"DST");
        else strcpy(xx->dststr,"Std");
	sprintf(str,"%02d:%02d:%02d %s %s",
	   xx->hh, xx->mm, xx->ss,
           xx->ampmstr, xx->dststr);
        XStoreName(xx->dpy,xx->w,str);
        XSetIconName(xx->dpy,xx->w,str);
	} /* gettim */

/* draw tick marks on the clock */

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

/* write the 12 hour numbers on the clock */

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
   XDrawString(xx->dpy, xx->w, xx->gc,
	   600, 40, xx->ampmstr, 2);
   if (xx->isdst == 0)
      {
      XDrawString(xx->dpy, xx->w, xx->gc,
	      600, 70, "Std", 3);
      } /* if AM */
   else if (xx->isdst == 1)
      {
      XDrawString(xx->dpy, xx->w, xx->gc,
	      600, 70, "DST", 3);
      } /* else PM */
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
   while (xx->runflg)
      {
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->redColor);
      /* x,y,wdth,hght,angle1,angle2 */
      XDrawArc(xx->dpy, xx->w, xx->gc,
	 xx->xc-xx->arr,xx->yc-xx->arr,
         xx->arr << 1, xx->arr << 1,
         0, xx->fullcrcl);
      /*******************************************************/
      /* draw the hub of the clock                           */
      /*******************************************************/
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->blackColor);
      XFillArc(xx->dpy, xx->w, xx->gc,
         xx->xc-10, xx->yc-10,
         20, 20, 0, xx->fullcrcl);
      gettim(xx);
      puthrs(xx);
      XSetForeground(xx->dpy, xx->gc, xx->redColor);
      putticks(xx);
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
      XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
      XSetForeground(xx->dpy, xx->gc, xx->whiteColor);
      /*******************************************************/
      /* eraase the second hand                              */
      /*******************************************************/
      XDrawLine(xx->dpy, xx->w, xx->gc,
         xx->xc, xx->yc, secexx, secwyy);
      /*******************************************************/
      /* eraase the minute hand                              */
      /*******************************************************/
      XDrawLine(xx->dpy, xx->w, xx->gc,
         xx->xc, xx->yc, minexx, minwyy);
      /*******************************************************/
      /* eraase the hour hand                                */
      /*******************************************************/
      XDrawLine(xx->dpy, xx->w, xx->gc,
         xx->xc, xx->yc, hrexx, hrwyy);
      /*******************************************************/
      /* flush the old time                                  */
      /*******************************************************/
      // XFlush(xx->dpy);
      ifkey(xx);
      } /* infinite loop for each second of time */
   } /* telltime */
  
/* main function */

int main() 
   { 
   xxfmt *xx;       /* global structure */
   /* sllocate global structure */
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of memory */
   initx(xx);      /* initialize X Windows */
   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->blackColor);
   xx->fullcrcl = FULLCIRCLE;
   /* length of three hands on clock */
   xx->arr    = ((xx->dpyhght-100) >> 1);     /* second hand */
   xx->minarr = xx->arr * 0.9;                /* minute hand */
   xx->hrarr  = xx->arr * 0.65;               /* hour   hand */
   xx->xc  = xx->yc = xx->arr + 50;
   xx->erase = 0;
   xx->runflg = xx->repeat = 1;
   xx->pi = M_PI;
   xx->twopi = 2.0 * xx->pi;
   telltime(xx);      /* tell time */
   XDestroyWindow(xx->dpy,xx->w);
   XCloseDisplay(xx->dpy);
   free(xx);
   return(0); 
   } 
