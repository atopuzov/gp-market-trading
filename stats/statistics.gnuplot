set encoding utf8
set terminal postscript eps enhanced
set grid
set border
#set xlabel  "Generation"
set xlabel  "Generacija"

#	Stats file data
#	1	2	3	4	5
#	Gen	Avg	Std	Max	Min

#set output "stats.ps"
#set multiplot
#set size 0.5,0.5

# Line styles
set style line 1 lc rgb "#eeeeee"
set style line 2 lt 1 lw 2 lc "black"
set style line 3 lt 3 lw 2 lc "black"
set style line 4 lt 4 lw 2 lc "black"
set style line 5 lt 1 lc "black" lw 1
set key box linestyle 5
#set key out vert
#set key right top
set key left top
set key spacing 1.5
#set nokey

set output "stats_fitness_ts.eps"

#set origin 0.0,0.5
#set ylabel  "Fitness"
#set title "Fitness on the training set"
set ylabel  "Dobrota"
#set title "Dobrota na intervalu za učenje"
plot 'stats_fitness_ts.dat'	using 1:2:($2+$3) title "Std" with filledcurves ls 1, \
	 'stats_fitness_ts.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_fitness_ts.dat'	using 1:2 title "Avg"  with lines ls 2, \
	 'stats_fitness_ts.dat'	using 1:4 title "Max"  with lines ls 3, \
	 'stats_fitness_ts.dat'	using 1:5 title "Min"  with lines ls 4

set output "stats_fitness_vs.eps"
#set origin 0.5,0.5
#set ylabel  "Fitness"
#set title "Fitness on the validation set"
set ylabel "Dobrota"
#set title "Dobrota na intervalu za provjeru"
plot 'stats_fitness_vs.dat'	using 1:2:($2+$3) title "Std" with filledcurves ls 1, \
	 'stats_fitness_vs.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_fitness_vs.dat'	using 1:2 title "Avg"  with lines ls 2, \
	 'stats_fitness_vs.dat'	using 1:4 title "Max"  with lines ls 3, \
	 'stats_fitness_vs.dat'	using 1:5 title "Min"  with lines ls 4

# Tree depth
set output "stats_treedepth.eps"
#set origin 0.0,0.0
#set ylabel "Levels"
#set title "Tree depth"
set ylabel "Nivoa"
#set title "Dubina stabla"
plot 'stats_treedepth.dat'	using 1:2:($2+$3) title "Std" with filledcurves ls 1, \
	 'stats_treedepth.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_treedepth.dat'	using 1:2 title "Avg"  with lines ls 2, \
	 'stats_treedepth.dat'	using 1:4 title "Max"  with lines ls 3, \
	 'stats_treedepth.dat'	using 1:5 title "Min"  with lines ls 4

# Tree size
set output "stats_treesize.eps"
#set origin 0.5,0.0
#set ylabel "Nodes"
#set title "Tree size"
set ylabel "Cvorova"
#set title "Veličina stabla"
plot 'stats_treesize.dat'	using 1:2:($2+$3) title "Std" with filledcurves ls 1, \
	 'stats_treesize.dat'	using 1:2:($2-$3) notitle with filledcurves ls 1, \
	 'stats_treesize.dat'	using 1:2 title "Avg"  with lines ls 2, \
	 'stats_treesize.dat'	using 1:4 title "Max"  with lines ls 3, \
	 'stats_treesize.dat'	using 1:5 title "Min"  with lines ls 4
