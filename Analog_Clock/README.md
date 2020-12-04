# Analog_Clock

There are four analog clocks in this repository:

clk - Analog clock showing time of day using the Bresenham algorithm

xclk - Analog clock showing time of day using XWindows routines

stopw - Stop watch timer

timer - Count down timer

Except for xclk, each of these clocks is written in XWindows
using Bresenham's algorithm to paint the circle and hands of
the clock.  Xclk uses standard XWindows routines to plot the
graphics on the clock.  Using top to measure performance,
xclk is more efficient than clk.

See TESTING for examples of how to run each of these clocks.

See INSTALL for instructions on how to compile these clocks.

See the Digital_Clock repository for three equivalent clocks
filling a console screen using curses with asterisks.
