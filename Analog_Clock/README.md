# Analog_Clock

There are six analog clocks in this repository:

clk - Analog clock showing time of day using the Bresenham algorithm

xclk - Analog clock showing time of day using XWindows routines

stopw - Stop watch timer using Bresenham's algorithm

xstopw - Stop watch timer using X Windows routines

timer - Count down timer using Bresenham's algorithm

xtimer - Count down timer using X Windows routines

All clocks starting with 'x' use X Windows routines.
All the other clocks use Bresenham's algorithm to paint
the circle and hands of the clock.
Using top to measure performance, X Windows clocks are more
efficient than Bresenham clocks.

See TESTING for examples of how to run each of these clocks.

See INSTALL for instructions on how to compile these clocks.

See the Digital_Clock repository for three equivalent clocks
filling a console screen using curses with asterisks.
