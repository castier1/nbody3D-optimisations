set terminal png size 800,500 enhanced font "Arial,12"
set output 'assembly.png'
set title "Courbes du nombre d'occurrences par instructions"
set grid x
set xtics ('vmulss' 0, 'vaddss' 1, 'vfmadd213ss' 2, 'vbroadcastss' 3, 'vmovss' 4, 'addq' 5, 'vpbroadcastq' 6, 'movq' 7, 'vmulps' 8, 'vaddps' 9, 'vfmadd231ps' 10, 'vfnmadd231ps' 11, 'vmovups' 12, 'vmovaps' 13, 'vrsqrt28ps' 14, 'vrsqrt28pd' 15) rotate by 45 right
set ylabel "Occurency"
set xlabel "Instructions"
set linestyle 1 lt 2 lw 2
plot "assembly_data/nbody0.data" w lp title "nbody0", "assembly_data/nbody1.data" w lp title "nbody1", "assembly_data/nbody2.data" w lp title "nbody2", "assembly_data/nbody3.data" w lp title "nbody3", "assembly_data/nbody4.data" w lp title "nbody4", "assembly_data/nbody5.data" w lp title "nbody5", "assembly_data/nbody6.data" w lp title "nbody6", "assembly_data/nbody7.data" w lp title "nbody7", "assembly_data/nbody8.data" w lp title "nbody8"
