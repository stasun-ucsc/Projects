set terminal pdf
set output "example.pdf"
set xlabel "x"
set ylabel "y"
plot "example.dat" with linespoints
