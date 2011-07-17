#/bin/bash
ts=`xmlstarlet sel -t -v /Individual/Fitness/FitnessTS ${1}`
vs=`xmlstarlet sel -t -v /Individual/Fitness/FitnessVS ${1}`
dir=`dirname ${1}`
obm="${dir}/beagle_g0.obm"
pop=`xmlstarlet sel -t -v "/Beagle/System/Register/Entry[@key='ec.pop.size']" ${obm}`
gen=`xmlstarlet sel -t -v "/Beagle/System/Register/Entry[@key='ec.term.maxgen']" ${obm}`
mut=`xmlstarlet sel -t -v "/Beagle/System/Register/Entry[@key='gp.mutstd.indpb']" ${obm}`
echo "${pop}\t${gen}\t${mut}\t${ts}\t${vs}"

