@ECHO OFF
DEL viz\*.html viz\*.svg viz\*.obm viz\*.log viz\*.csv
REM utils\gzip -df beagle.obm.gz
REM utils\beagleviz -b http://localhost/visualizer -o viz beagle.obm beagle.log
utils\beagleviz -o viz beagle.obm beagle.log
start viz\index.html
