// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// #define N 100000
#define SEED 72
struct point { double x, y; };

int main(int argc, char *argv[])
{	
  // Take epsilon as command line input:
  if (argc != 2)
  { printf("\nIncorrect number of input parameters. Please input an epsilon distance.\n");
	  return 0;
  } 
  double epsilon = atof(argv[1]);
  
  // Initialize point values between [0-1] for x and y:  
  struct point * pointData;
  pointData = (struct point *)malloc(sizeof(struct point) * N); 
  // Display memory required for pointData in mebibytes:
  printf("\nSize of the points data set (MiB): %f", (2.0 * sizeof(double) * N * 1.0) / (1024.0 * 1024.0)); 
  // In gibibytes: (N * 1.0 * sizeof(struct point)/(1024.0 * 1024.0 * 1024.0)) 
  // Seed the rng:
  srand(SEED);
  for (unsigned int i = 0; i < N; i++)
  {
    pointData[i].x = 1000.0 * ((double)(rand()) / RAND_MAX); 
    pointData[i].y = 1000.0 * ((double)(rand()) / RAND_MAX);	 
  }
  // Check the points:	
  // for(int i = 0; i < N; ++i)
  //   printf("Point #%d: (%f, %f)\n", i, pointData[i].x, pointData[i].y);  

  /*---
   CPU	
  ---*/

  unsigned long long int countDistancesLessThanEpsilon = 0;
  double distance, tstartCPU = omp_get_wtime();
  int i, j;  

  // Set thread count to 16 for 16 cores (1 thread per core):
  omp_set_num_threads(16);
  printf("\nTotal number of cores (CPU): %d, Number of maximum threads: %d", omp_get_num_procs(), omp_get_max_threads());

  #pragma omp parallel for private(i, j, distance) reduction(+:countDistancesLessThanEpsilon)
  for(i = 0; i < N; ++i)
  {
    for(j = 0; j < N; ++j)
    {   
      distance = sqrt(pow((pointData[i].x - pointData[j].x), 2) + pow((pointData[i].y - pointData[j].y), 2));
      if(distance < epsilon)
		    countDistancesLessThanEpsilon++;
    }  
  }  
  double tendCPU = omp_get_wtime();

  printf("\nTotal time taken by the CPU (in seconds): %f", tendCPU - tstartCPU);
  printf("\nTotal number of distances between points that are within a search radius (epsilon) of %.1f, as computed by the CPU: %llu", epsilon, countDistancesLessThanEpsilon);
  
  free(pointData);
  return 0;
}