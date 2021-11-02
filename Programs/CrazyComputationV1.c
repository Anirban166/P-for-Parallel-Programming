// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

#include <omp.h>
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>

enum { SIZE = 50 };
double do_crazy_computation(int i, int j);

int main(int argc, char **argv) {
  double mat[SIZE][SIZE];
  int i, j;
  //double timings[10], timingsSum = 0;
  omp_set_num_threads(2);

  //for(i = 0; i < 10; i++) {
  double tstart = omp_get_wtime();

  #pragma omp parallel for private(i, j) shared(mat) schedule(static) //collapse(2)
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      mat[i][j] = do_crazy_computation(i, j);
      fprintf(stderr, ".");
    }
  }

  double tend = omp_get_wtime();
  double elapsedTime = tend - tstart;
  //timings[i] = tend - tstart;
  //timingsSum += timings[i];
  printf("\nElapsed time: %f seconds\n", elapsedTime);  
  //}
  //for(i = 0; i < 10; i++)
  //{
  //  printf("\nElapsed time for run #%d: %f seconds", i+1, timings[i]);
  //}
  //printf("\nOverall Execution time (average of the ten runs above): %f seconds\n", (timingsSum/10.0));
  exit(0);
}

double do_crazy_computation(int x, int y) {
   int iter;
   double value = 0.0;
   for (iter = 0; iter < 5*x*x*x+1 + y*y*y+1; iter++) {
     value += (cos(x*value) + sin(y*value));
   }
  return value;
}
