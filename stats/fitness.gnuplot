set terminal postscript eps enhanced
set grid
set border
set xlabel  "Generation"
set ylabel  "Fitness"

#set style line 1 lt 1 lw 2 lc "black"
#set style line 2 lt 2 lw 4 lc "black"
set style line 1 lc rgb "#eeeeee"
set style line 2 lc rgb "#dddddd"

set output "aco1.ps"
set title "Traing set fitness vs. validation set fitness"
#plot 'stats_fitness.dat'	using 1:2 title "training set (avg)" with lines ls 1, \
#	 'stats_fitness.dat'	using 1:3 title "validation set (avg)" with lines ls 2
	
	
plot 'stats_fitness.dat'	using 1:2:($2+$3) notitle with filledcurves ls 1, \
	 'stats_fitness.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_fitness.dat'	using 1:5:($5+$6) notitle with filledcurves ls 2, \
	 'stats_fitness.dat'	using 1:5:($5-$6) notitle with filledcurves ls 2, \
	 'stats_fitness.dat'	using 1:2 title "training set (avg)" with lines lt 1, \
	 'stats_fitness.dat'	using 1:4 title "training set (max)" with lines lt 3, \
	 'stats_fitness.dat'	using 1:5 title "validation set (avg)" with lines lt 2, \
	 'stats_fitness.dat'	using 1:7 title "validation set (max)" with lines lt 4
	
