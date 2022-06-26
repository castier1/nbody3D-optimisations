//
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//
typedef float              f32;
typedef double             f64;
typedef unsigned long long u64;


typedef struct {
  f32 *x, *y, *z;
} array;


//
void init(array particle, array particle_v, u64 n)
{
  for (u64 i = 0; i < n; i++)
    {
      //
      u64 r1 = (u64)rand();
      u64 r2 = (u64)rand();
      f32 sign = (r1 > r2) ? 1 : -1;
      
      //
      particle.x[i] = sign * (f32)rand() / (f32)RAND_MAX;
      particle.y[i] = (f32)rand() / (f32)RAND_MAX;
      particle.z[i] = sign * (f32)rand() / (f32)RAND_MAX;

      //
      particle_v.x[i] = (f32)rand() / (f32)RAND_MAX;
      particle_v.y[i] = sign * (f32)rand() / (f32)RAND_MAX;
      particle_v.z[i] = (f32)rand() / (f32)RAND_MAX;
    }
}

//
void move_particles(array particle, array particle_v, const f32 dt, u64 n)
{
  //
  const f32 softening = 1e-20;
  //
  for (u64 i = 0; i < n; i++)
  {
    //
    f32 fx = 0.0;
    f32 fy = 0.0;
    f32 fz = 0.0;

    const f32 pxi = particle.x[i];
    const f32 pyi = particle.y[i];
    const f32 pzi = particle.z[i];

    //25 floating-point operations
    for (u64 j = 0; j < n; j++)
    {
  	  //Newton's law
  	  const f32 dx = particle.x[j] - pxi; //1
  	  const f32 dy = particle.y[j] - pyi; //2
  	  const f32 dz = particle.z[j] - pzi; //3
  	  const f32 d_2 = (dx * dx) + (dy * dy) + (dz * dz) + softening; //9
      const f32 d_1_over_2 = pow(d_2, 1.0 / 2.0); //11
  	  const f32 d_3_over_2 = d_1_over_2 * d_1_over_2 * d_1_over_2; //13

  	  //Net force
  	  fx += dx / d_3_over_2; //15
  	  fy += dy / d_3_over_2; //17
  	  fz += dz / d_3_over_2; //19
  	}

    //
    particle_v.x[i] += dt * fx; //21
    particle_v.y[i] += dt * fy; //23
    particle_v.z[i] += dt * fz; //25
  }

  //3 floating-point operations
  for (u64 i = 0; i < n; i++)
  {
    particle.x[i] += dt * particle_v.x[i];
    particle.y[i] += dt * particle_v.y[i];
    particle.z[i] += dt * particle_v.z[i];
  }

}

void write_perf_in_file(char *filename, f64 data)
{
  FILE *file;
  file = fopen(filename, "a");
  if(!file){
    printf("ERROR: write_perf_in_file: cannot open file.\n");
    return;
  }
  fprintf(file, "2 %lf\n", data);
  fclose(file);
}

//
int main(int argc, char **argv)
{
  //
  const u64 n = (argc > 1) ? atoll(argv[1]) : 16384;
  const u64 steps = 10;
  const f32 dt = 0.01;

  //
  f64 rate = 0.0, drate = 0.0;

  //Steps to skip for warm up
  const u64 warmup = 3;
  
  //
  array particle, particle_v;

  particle.x = malloc(sizeof(f32) * n);
  particle.y = malloc(sizeof(f32) * n);
  particle.z = malloc(sizeof(f32) * n);
  particle_v.x = malloc(sizeof(f32) * n);
  particle_v.y = malloc(sizeof(f32) * n);
  particle_v.z = malloc(sizeof(f32) * n);

  //
  init(particle, particle_v, n);

  const u64 s = sizeof(f32) * 6;
  
  printf("\n\033[1mTotal memory size:\033[0m %llu B, %llu KiB, %llu MiB\n\n", s, s >> 10, s >> 20);

  //
  printf("\033[1m%5s %10s %10s %8s\033[0m\n", "Step", "Time, s", "Interact/s", "GFLOP/s"); fflush(stdout);

  //
  for (u64 i = 0; i < steps; i++)
  {
    //Measure
    const f64 start = omp_get_wtime();

    move_particles(particle, particle_v, dt, n);

    const f64 end = omp_get_wtime();

    //Number of interactions/iterations
    const f32 h1 = (f32)(n) * (f32)(n - 1);

    //GFLOPS
    const f32 h2 = (25.0 * h1 + 3.0 * (f32)n) * 1e-9;
    
    if (i >= warmup)
    {
      rate += h2 / (end - start);
      drate += (h2 * h2) / ((end - start) * (end - start));
    }

    //
    printf("%5llu %10.3e %10.3e %8.1f %s\n",
     i,
     (end - start),
     h1 / (end - start),
     h2 / (end - start),
     (i < warmup) ? "*" : "");
    
    fflush(stdout);
  }

  //
  rate /= (f64)(steps - warmup);
  drate = sqrt(drate / (f64)(steps - warmup) - (rate * rate));

  printf("-----------------------------------------------------\n");
  printf("\033[1m%s %4s \033[42m%10.1lf +- %.1lf GFLOP/s\033[0m\n",
	 "Average performance:", "", rate, drate);
  printf("-----------------------------------------------------\n");
  
  write_perf_in_file("../dat.data", rate);
  //
  free(particle.x);
  free(particle.y);
  free(particle.z);
  free(particle_v.x);
  free(particle_v.y);
  free(particle_v.z);

  //
  return 0;
}
