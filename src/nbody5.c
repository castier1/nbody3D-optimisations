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

    //42 floating-point operations
    for (u64 j = 0; j < n; j++)
    {
      //Newton's law
      const f32 pxj = particle.x[j];
      const f32 pyj = particle.y[j];
      const f32 pzj = particle.z[j];

      const f32 dx = (pxj * pxj) - (2 * pxi * pxj) + (pxi * pxi); //6
      const f32 dy = (pyj * pyj) - (2 * pyi * pyj) + (pyi * pyi); //12
      const f32 dz = (pzj * pzj) - (2 * pzi * pzj) + (pzi * pzi); //18
      const f32 d_2 = dx + dy + dz + softening; //21
      const f32 d_1_over_2 = sqrt(d_2); //22
      const f32 d_3_over_2 = d_1_over_2 * d_1_over_2 * d_1_over_2; //24

      //Net force
      fx += dx / d_3_over_2; //26
      fy += dy / d_3_over_2; //28
      fz += dz / d_3_over_2; //30
    }

    //
    particle_v.x[i] += dt * fx; //32
    particle_v.y[i] += dt * fy; //34
    particle_v.z[i] += dt * fz; //36
    
    particle.x[i] += dt * particle_v.x[i]; //38
    particle.y[i] += dt * particle_v.y[i]; //40
    particle.z[i] += dt * particle_v.z[i]; //42
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
  fprintf(file, "5 %lf\n", data);
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
    const f32 h2 = (42.0 * h1) * 1e-9;
    
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
