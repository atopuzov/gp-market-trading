#!/bin/bash
#***************************************************************************
#* (c) 2008-2011 Aleksandar Topuzović                                      *
#* <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
#***************************************************************************
# Create a script to run tests with multiple parametars
populations=("10" "20" "30" "50" "100" "200")
generations=("5" "10" "15" "20" "30")
stdmutations=("0.01" "0.05" "0.1")
step=1
dir="/home/aco/devel/gp-market-trading"
prog="${dir}/prog.exe"
db="${dir}/se.db"
ob="-OBec.conf.file=${dir}/test/prog.conf,trading.database=${db},"

# Population sizes
for pop in "${populations[@]}"
do
	obpop="ec.pop.size=${pop}"
	fnpop="p${pop}"
	# Generations
	for gen in "${generations[@]}"
	do
		obgen="ec.term.maxgen=${gen}"
		fngen="g${gen}"
		# Mutations
		for mut in "${stdmutations[@]}"
		do
			obmut="gp.mutstd.indpb=${mut}"
			fnmut="sm${mut}"
			dname="${step}-${fnpop}-${fngen}-${fnmut}"
			params="${ob}${obpop},${obgen},${obmut}"
			echo mkdir ${dname}
			echo cd ${dname}
			echo nice -n 20 ${prog} ${params}
			echo cd ..
			let step=step+1
		done
	done
done
