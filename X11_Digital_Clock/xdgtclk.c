/*
xdgtclk.c Version 0.3.0. Digital Clock For X Windows.
Copyright (C) 2003-2021  https://github.com/aquila57

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

/*
xdgtclk.c Version 0.2.0. Digital Clock For X Windows.
Copyright (C) 2003-2008  dondalah721@yahoo.com (Dondalah)

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

/* to define the escape key */
#define XK_MISCELLANY 1 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <assert.h>
#include <unistd.h>

/* The purpose of this digital clock is to maintain */
/* a small footprint with an active icon.  If you want */
/* a larger font, use the Debian package dclock */

/* Debian font is defined in xdgtclk.mak */
/* #define FONT "12x24" */

#define NIL (0)
#define SCRW 420
#define SCRH 40
#define ROW 30
#define COL 4

int whiteColor,blackColor;
int runflg,timesw,resetsw,secondsw;
int prevmin,prevsec;
time_t frstsec;
unsigned long blue;
Display *dpy;
Window w;
GC gc;
Font fontid;
Screen *dfltscr;
Colormap cmap;
XColor scrdef,exctdef;

/* X Windows code is based on: */
/* http://tronche.lri.fr:8000/gui/x/xlib-tutorial/2nd-program-anatomy.html */

void getkey()
   {
   int i,rslt,msk;
   int symbol;
   int XCheckMaskEvent();
   XEvent e;
   XKeyEvent *k;
   char spc[32];

   msk = KeyPressMask|ExposureMask;

   XSelectInput(dpy, w, msk);

   while (XCheckMaskEvent(dpy, msk, &e))
      {
      if (e.type == KeyPress)
         {
         k = (XKeyEvent *) &e;
         symbol = XLookupKeysym(k,0);
         resetsw = 0;
         if (symbol == XK_Escape || symbol == 'q')
            {
            runflg = 0;
            } /* if quit */
         else if (symbol == 's')
            {
            secondsw = (secondsw ^ 1) & 1;
	    prevmin = prevsec = -999999;
            } /* secondsw toggle */
         else if (symbol == 't')
            {
            timesw ^= 1;
            } /* toggle time mode */
         else if (symbol == 'r')
            {
            resetsw = 1;
            } /* reset */
         } /* if keypress event */
      else if (e.type == Expose)
         {
         prevmin = prevsec = -99999;
         } /* if expose event */
      } /* if event received */
   } /* getkey */

void initx()
   {
   int i,rslt;
   char title[64];

   dpy = XOpenDisplay(NIL);

   if (dpy == NULL)
      {
      fprintf(stderr,"X Windows failure\n");
      exit(1);
      } /* if X Windows is not active */

   assert(dpy);

   whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
   blackColor = BlackPixel(dpy, DefaultScreen(dpy));

   w = XCreateSimpleWindow(dpy,
      DefaultRootWindow(dpy),
      0, 0, 
      SCRW, SCRH,
      0, whiteColor,
      whiteColor);

   XSelectInput(dpy, w, StructureNotifyMask);

   XMapWindow(dpy, w);

   gc = XCreateGC(dpy, w, 0, NIL);

   fontid = (Font) XLoadFont(dpy,FONT);

   XSetFont(dpy,gc,fontid);

   XSetBackground(dpy, gc, whiteColor);
   XSetForeground(dpy, gc, blackColor);

   dfltscr = XDefaultScreenOfDisplay(dpy);
   if (dfltscr == NULL)
      {
      fprintf(stderr,"XDefaultScreenOfDisplay failed\n");
      perror("XDefaultScreenOfDisplay failed");
      exit(1);
      } /* if error */

   cmap = XDefaultColormapOfScreen(dfltscr);

   rslt = XAllocNamedColor(dpy,cmap,"blue",
      &scrdef,&exctdef);
   if (rslt < 0)
      {
      fprintf(stderr,"XAllocNamedColor failed\n");
      perror("XAllocNamedColor failed");
      exit(1);
      } /* if error */
   blue = scrdef.pixel;

   XSetWindowBorderWidth(dpy, w, 40);

   sprintf(title,"xdgtclk");
   XStoreName(dpy,w,title);
   XSetIconName(dpy,w,title);

#ifdef UPPERLEFT
   XMoveWindow(dpy,w,0,0);
#endif

   while(1)
      {
      XEvent e;
      XNextEvent(dpy, &e);
      if (e.type == MapNotify) break;
      } /* wait for window initialization */

   } /* initx */

void putascii(year,jul,dst,lcltm,str)
int year,jul,dst;
struct tm *lcltm;
char *str;
   {
   int len;
   char julian[128];
   char alfadate[64];
   char title[64];
   char spc[32];
   strcpy(alfadate,str);
   len = strlen(alfadate);
   alfadate[len-1] = '\0';
   strcat(alfadate,"   ");
   len = strlen(alfadate);
   strcpy(spc,"                    ");

   if (secondsw)
      {
      if (lcltm->tm_sec != prevsec)
	 {
         sprintf(title,"%02d:%02d:%02d",
            lcltm->tm_hour, lcltm->tm_min,
	    lcltm->tm_sec);
         XStoreName(dpy,w,title);
         XSetIconName(dpy,w,title);
         } /* current second changed */
      } /* if show seconds */
   else
      {
      if (lcltm->tm_min != prevmin)
         {
         sprintf(title,"%02d:%02d",
            lcltm->tm_hour, lcltm->tm_min);
         XStoreName(dpy,w,title);
         XSetIconName(dpy,w,title);
         } /* current minute changed */
      } /* else if show minutes only */

   XDrawImageString(dpy,w,gc,COL,ROW,alfadate,len);

   sprintf(julian,"  JD %3d ", jul);
   XDrawImageString(dpy,w,gc,COL+315,ROW,spc,20);
   XDrawImageString(dpy,w,gc,COL+415,ROW,spc,20);
   XDrawImageString(dpy,w,gc,COL+230,ROW,julian,strlen(julian));

   XDrawImageString(dpy,w,gc,COL+370,ROW,spc,20);
   XDrawImageString(dpy,w,gc,COL+470,ROW,spc,20);
   if (dst)
      {
      XDrawImageString(dpy,w,gc,COL+340,ROW," DST",4);
      } /* if daylight saving */
   else
      {
      XDrawImageString(dpy,w,gc,COL+340,ROW," STD",4);
      } /* if standard time */

   } /* putascii */

void runclk()
   {
   int curryr,relsecs;
   time_t now;
   clock_t clk;
   struct tms t;
   struct tm *lcltm;
   struct tm reltm;
   char *str;
   /* get clock ticks since boot */
   clk = times(&t);
   runflg = 1;
   while (runflg)
      {
      /* get date & time */
      time(&now);
      if (timesw) /* if relative time */
         {
         relsecs = now - frstsec;
         lcltm = &reltm;
         lcltm->tm_year = -1900;
         lcltm->tm_mon  = lcltm->tm_wday
            = lcltm->tm_yday = lcltm->tm_isdst = 0;
         lcltm->tm_mday  = 1;
         lcltm->tm_hour = relsecs / 3600;
         lcltm->tm_min  = (relsecs % 3600) / 60;
         lcltm->tm_sec  = relsecs % 60;
         } /* if relative time */
      else
         lcltm = (struct tm *) localtime(&now);
      curryr = lcltm->tm_year+1900;
      str = (char *) asctime(lcltm);
      putascii(curryr,
         lcltm->tm_yday+1,
         lcltm->tm_isdst,
         lcltm,
         str);
      prevmin = lcltm->tm_min;
      prevsec = lcltm->tm_sec;
      getkey();
      if (runflg) sleep(1);
      if (resetsw)
         {
         resetsw = 0;
         /* if relative time */
         if (timesw) time(&frstsec);
         } /* if reset */
      } /* second loop */
   } /* runclk */

int main()
   {
   prevmin = prevsec = -99999;
   initx();
   secondsw = timesw = resetsw = 0;
   time(&frstsec);
   runclk();
   return(0);
   } /* main */
