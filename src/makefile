.PHONY: all clean

build_gcc: nbody0.g nbody1.g nbody2.g nbody3.g nbody4.g nbody5.g nbody6.g nbody7.g nbody8.g
build_icc: nbody0.i nbody1.i nbody2.i nbody3.i nbody4.i nbody5.i nbody6.i nbody7.i nbody8.i
build_sicc: nbody0.s nbody1.s nbody2.s nbody3.s nbody4.s nbody5.s nbody6.s nbody7.s nbody8.s

%.g: %.c
	gcc -march=knl -mtune=knl -mavx2 -Ofast -funroll-loops -finline-functions -ftree-vectorize -fopt-info-all=nbody.gcc.optrpt -g $< -o $@ -lm -fopenmp

nbody0.g: nbody0.c
nbody1.g: nbody1.c
nbody2.g: nbody2.c
nbody3.g: nbody3.c
nbody4.g: nbody4.c
nbody5.g: nbody5.c
nbody6.g: nbody6.c
nbody7.g: nbody7.c
nbody8.g: nbody8.c



%.i: %.c
	icc -xhost -mavx2 -Ofast -ipo -funroll-all-loops -finline-functions -qopt-report -g $< -o $@ -qmkl -qopenmp

nbody0.i: nbody0.c
nbody1.i: nbody1.c
nbody2.i: nbody2.c
nbody3.i: nbody3.c
nbody4.i: nbody4.c
nbody5.i: nbody5.c
nbody6.i: nbody6.c
nbody7.i: nbody7.c
nbody8.i: nbody8.c



%.s: %.c
	icc -xhost -mavx2 -Ofast -ipo -funroll-all-loops -finline-functions -qopt-report $< -S -o $@ -qmkl -qopenmp

nbody0.s: nbody0.c
nbody1.s: nbody1.c
nbody2.s: nbody2.c
nbody3.s: nbody3.c
nbody4.s: nbody4.c
nbody5.s: nbody5.c
nbody6.s: nbody6.c
nbody7.s: nbody7.c
nbody8.s: nbody8.c

clean:
	rm -Rf *~ *.g *.i *.s *.optrpt

