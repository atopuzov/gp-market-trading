set terminal postscript eps enhanced
set grid
set border
set ylabel  "Fitness"
set pointsize 2
set key box
set key left top
set key spacing 1.5

#	pop	gen	mut	ts	vs

set output "results_population.eps" 
set xrange [0:110]
set xlabel  "Population"
plot	'rez.txt'	using 1:4 title "Training", \
		'rez.txt'	using 1:5 title "Validation"

set output "results_generation.eps" 
set xrange [0:35]
set xlabel  "Generation"
plot	'rez.txt'	using 2:4 title "Training", \
		'rez.txt'	using 2:5 title "Validation"

set output "results_mutation.eps" 
set xrange [0:0.11]
set xlabel  "Mutation"
plot	'rez.txt'	using 3:4 title "Training", \
		'rez.txt'	using 3:5 title "Validation"


