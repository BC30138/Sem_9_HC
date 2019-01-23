set xlabel 'Data size (Byte)'
set ylabel 'Time (s)'
set key spacing 1.5
set key right
plot 'data/time.dat' u 1:2 w lines lw 2 lt rgb 'red'
pause -1
