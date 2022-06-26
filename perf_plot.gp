set grid y
set terminal png size 800,600 enhanced font "Arial,12"
set output 'perf.png'
set xlabel "Versions"
set ylabel "Gflop/s"
set title "Histogramme des taux de Gflop/s pour chacunes des versions du programme"
set style histogram clustered gap 1 title textcolor lt -1
set datafile missing '-'
set style data histograms
plot './dat.data' using 2:xtic(1) with histogram title '', '' using 0:2:2 with labels title ''
