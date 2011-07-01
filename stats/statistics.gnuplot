set terminal postscript eps enhanced
set grid
set border
set xlabel  "Generation"

#	Stats file data
#	1	2	3	4	5
#	Gen	Avg	Std	Max	Min

set output "stats.ps"
set multiplot

# Line styles
set style line 1 lc rgb "#eeeeee"
set style line 2 lt 1 lw 2 lc "black"
set style line 3 lt 3 lw 2 lc "black"

#set output "stats_fitness_ts"
set size 0.5,0.5
set origin 0.0,0.5
set ylabel  "Fitness"
set title "Fitness on the training set"
plot 'stats_fitness_ts.dat'	using 1:2:($2+$3) notitle with filledcurves ls 1, \
	 'stats_fitness_ts.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_fitness_ts.dat'	using 1:2 notitle  with lines ls 2, \
	 'stats_fitness_ts.dat'	using 1:4 notitle  with lines ls 3, \
	 'stats_fitness_ts.dat'	using 1:5 notitle  with lines ls 3

#set output "stats_fitness_vs.ps"
set size 0.5,0.5
set origin 0.5,0.5
set ylabel  "Fitness"
set title "Fitness on the validation set"
plot 'stats_fitness_vs.dat'	using 1:2:($2+$3) notitle with filledcurves ls 1, \
	 'stats_fitness_vs.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_fitness_vs.dat'	using 1:2 notitle  with lines ls 2, \
	 'stats_fitness_vs.dat'	using 1:4 notitle  with lines ls 3, \
	 'stats_fitness_vs.dat'	using 1:5 notitle  with lines ls 3

# Tree depth
#set output "stats_treedepth.ps"
set size 0.5,0.5
set origin 0.0,0.0
set ylabel "Levels"
set title "Tree depth"
plot 'stats_treedepth.dat'	using 1:2:($2+$3) notitle with filledcurves ls 1, \
	 'stats_treedepth.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_treedepth.dat'	using 1:2 notitle  with lines ls 2, \
	 'stats_treedepth.dat'	using 1:4 notitle  with lines ls 3, \
	 'stats_treedepth.dat'	using 1:5 notitle  with lines ls 3

# Tree size
#set output "stats_treesize.ps"
set size 0.5,0.5
set origin 0.5,0.0
set ylabel "Nodes"
set title "Tree size"
plot 'stats_treesize.dat'	using 1:2:($2+$3) notitle with filledcurves ls 1, \
	 'stats_treesize.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_treesize.dat'	using 1:2 notitle  with lines ls 2, \
	 'stats_treesize.dat'	using 1:4 notitle  with lines ls 3, \
	 'stats_treesize.dat'	using 1:5 notitle  with lines ls 3

