#! /bin/bash

# A significant amount of this code I got from the official gnuplot 5.0 manual and demos.

make format
make clean && make monte_carlo # Rebuild the monte_carlo executable .
./monte_carlo -n 1000 > /tmp/monte_carlo.dat # Place the data points into a file .
./monte_carlo -n 25000 > /tmp/monte_carlo1.dat
sleep 1 
./monte_carlo -n 25000 > /tmp/monte_carlo2.dat
sleep 1
./monte_carlo -n 25000 > /tmp/monte_carlo3.dat
sleep 1
./monte_carlo -n 25000 > /tmp/monte_carlo4.dat
sleep 1
./monte_carlo -n 25000 > /tmp/monte_carlo5.dat

# This is the here - document that is sent to gnuplot .
gnuplot <<END
set terminal pdf
set output "monte_carlo_1.pdf"
# set title "Monte Carlo Fig. 2"
# set xlabel "x"
# set ylabel "y"
set palette defined (0 "red", 1 "blue") # from gnuplot demos
set object 1 circle at 0,0 size 1 back clip # from gnuplot manual
set xrange [0:1]
set yrange [0:1]
set xtics scale 0
set ytics scale 0
set size square
set pointsize 0.25
unset colorbox
plot "/tmp/monte_carlo.dat" using 3:4:5 with points pt 7 palette title "" # From John Yu's discussion slides

reset

set terminal pdf
set output "monte_carlo_2.pdf"
set title "Monte Carlo Error Estimation"
# set xlabel "Iterations"
set ylabel "Error"
set logscale x 4
set yrange [-1:1]
set xtics in scale 1,0
set grid xtics back
set grid ytics back
plot "/tmp/monte_carlo1.dat" using 1:(column(2)-pi) with lines title "",\
"/tmp/monte_carlo2.dat" using 1:(column(2)-pi) with lines title "",\
"/tmp/monte_carlo3.dat" using 1:(column(2)-pi) with lines title "",\
"/tmp/monte_carlo4.dat" using 1:(column(2)-pi) with lines title "",\
"/tmp/monte_carlo5.dat" using 1:(column(2)-pi) with lines title ""
END
