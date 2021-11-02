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
  int i, j, tid;
  double thread1EndTime = 0.0, thread2EndTime = 0.0;
  omp_set_num_threads(2);

  double tstart = omp_get_wtime();
  
  #pragma omp parallel private(i, j, tid) shared(mat, thread1EndTime, thread2EndTime)
  { 
    tid = omp_get_thread_num();  
    
    #pragma omp for schedule(static) nowait
    for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) { 
        mat[i][j] = do_crazy_computation(i, j);
        fprintf(stderr, ".");
      }
    }
    if(!tid) // tid == 0, first thread
    { thread1EndTime = omp_get_wtime(); }
    else     // tid == 1, second thread
    { thread2EndTime = omp_get_wtime(); }
  }
  
  double tend = omp_get_wtime();
  double elapsedTime = tend - tstart;
  double elapsedTimeThreadOne = thread1EndTime - tstart;
  double elapsedTimeThreadTwo = thread2EndTime - tstart;
  double loadImbalance = (elapsedTimeThreadOne > elapsedTimeThreadTwo) ? (elapsedTimeThreadOne - elapsedTimeThreadTwo) : (elapsedTimeThreadTwo - elapsedTimeThreadOne);

  printf("\nTotal Time (sanity check): %f seconds\n", elapsedTime);
  printf("Time Thread1: %f seconds\n", elapsedTimeThreadOne);
  printf("Time Thread2: %f seconds\n", elapsedTimeThreadTwo);
  printf("Load imbalance: %f seconds\n", loadImbalance);

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
