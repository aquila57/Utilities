/* clock.h */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <curses.h>

typedef struct xxstruct {
   int year;
   int month;
   int day;
   int hour;
   int minute;
   int second;
   int endsec;
   int currsec;
   time_t now;                 /* current date and time */
   clock_t clk;                /* current number of ticks */
   struct tms t;               /* structure used by times() */
   int ampm;                   /* 0 = am, 1 = pm */
   char *font;
   } xxfmt;

#define SPD (86400.0)

void gethms(xxfmt *xx);
void tm2jd(xxfmt *xx);
void jd2gr(xxfmt *xx);
void jd2tm(xxfmt *xx);
double jdcalc(int year, int month, int day,
   int hour, int minute, int second);
int getbyte(void);
void ldfont(xxfmt *xx);
void initcrss(void);
void putscr(xxfmt *xx);
