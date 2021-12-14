// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <complex.h>

// #define N 100000
#define SEED 72
#define BLOCKSIZE 1024

using namespace std; 
struct point { double x, y; };

void warmUpGPU();
__global__ void kernelFunction(struct point * pointData, unsigned long long int * countDistancesLessThanEpsilon, double epsilon);

int main(int argc, char *argv[])
{
  warmUpGPU();	

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

  /*---
   GPU	
  ---*/

  double tstart = omp_get_wtime();
  cudaError_t errCode = cudaSuccess;
  if(errCode != cudaSuccess)
  {  
    cout << "\nLast error: " << errCode << endl; 
  }

  struct point *dev_pointData;
  unsigned long long int *countDistancesLessThanEpsilon;
  unsigned long long int *dev_countDistancesLessThanEpsilon; 
  countDistancesLessThanEpsilon = (unsigned long long int *)malloc(sizeof(unsigned long long int)); 
  dev_countDistancesLessThanEpsilon = (unsigned long long int *)malloc(sizeof(unsigned long long int));
  *countDistancesLessThanEpsilon = 0;

  // Allocate the entire point data set (pointData) on the device:
  errCode = cudaMalloc((struct point**)&dev_pointData, sizeof(struct point) * N);	
  if(errCode != cudaSuccess) 
  {
	cout << "\nError: pointData error with code " << errCode << endl; 
  }
  // Allocate the counter variable (countDistancesLessThanEpsilon) on the device:
  errCode = cudaMalloc((unsigned long long int**)&dev_countDistancesLessThanEpsilon, sizeof(unsigned long long int));	
  if(errCode != cudaSuccess) 
  {
	cout << "\nError: countDistancesLessThanEpsilon error with code " << errCode << endl; 
  }	

  // Copy the point data set to device:
  errCode = cudaMemcpy(dev_pointData, pointData, sizeof(struct point) * N, cudaMemcpyHostToDevice);
  if(errCode != cudaSuccess) 
  {
	cout << "\nError: dev_pointData Memcpy error with code " << errCode << endl; 
  }	
  // Copy the counter variable to device:
  errCode = cudaMemcpy(dev_countDistancesLessThanEpsilon, countDistancesLessThanEpsilon, sizeof(unsigned long long int), cudaMemcpyHostToDevice);
  if(errCode != cudaSuccess) 
  {
	cout << "\nError: countDistancesLessThanEpsilon Memcpy error with code " << errCode << endl; 
  }

  // Calculate block count:
  const unsigned int totalBlocks = ceil(N * 1.0 / 1024.0);
  printf("\nTotal number of blocks (GPU): %d", totalBlocks);

  // Execute kernel:
  double tstartkernel = omp_get_wtime();
  kernelFunction<<<totalBlocks, BLOCKSIZE>>>(dev_pointData, dev_countDistancesLessThanEpsilon, epsilon);
	
  cudaDeviceSynchronize();
  double tendkernel = omp_get_wtime();
  printf("\nExecution time for only the GPU kernel (in seconds): %f", tendkernel - tstartkernel);

  if(errCode != cudaSuccess)
  {
	cout << "\nError after kernel launch! " << errCode << endl;
  }

  // Copy counter from device (GPU) back to the host (CPU):
  errCode = cudaMemcpy(countDistancesLessThanEpsilon, dev_countDistancesLessThanEpsilon, sizeof(unsigned long long int), cudaMemcpyDeviceToHost);
  if(errCode != cudaSuccess) 
  {
	cout << "\nError: getting result form GPU error with code " << errCode << endl; 
  }

  printf("\nTotal number of distances between points that are within a search radius (epsilon) of %.1f, as computed by the GPU: %llu", epsilon, *countDistancesLessThanEpsilon);
  double tend = omp_get_wtime();
  printf("\nTotal time taken by the GPU (in seconds): %f", tend - tstart);
  
  free(pointData);
  return 0;
}

__global__ void kernelFunction(struct point * pointData, unsigned long long int * countDistancesLessThanEpsilon, double epsilon) 
{
  unsigned int tid = threadIdx.x + (blockIdx.x * blockDim.x); 
  double distance;	
  if (tid >= N)
  {
	  return;
  }
  for(int i = 0; i < N; ++i)
  { 
    distance = sqrt(pow((pointData[tid].x - pointData[i].x), 2) + pow((pointData[tid].y - pointData[i].y), 2));
    if(distance < epsilon)
      atomicAdd(countDistancesLessThanEpsilon, int(1));
  }
  return;
}

__global__ void warmup(unsigned int * tmp) 
{
  if (threadIdx.x == 0)
  *tmp = 69;
  return;
}

void warmUpGPU()
{
  unsigned int * dev_tmp;
  unsigned int * tmp;
  tmp = (unsigned int*)malloc(sizeof(unsigned int));
  *tmp = 0;
  cudaError_t errCode = cudaSuccess;
  errCode = cudaMalloc((unsigned int**)&dev_tmp, sizeof(unsigned int));	
  if(errCode != cudaSuccess)
  {
	  cout << "\nError: dev_tmp error with code " << errCode << endl; 
  }
  warmup<<<1, BLOCKSIZE/4>>>(dev_tmp);
  // Copy data from device to host: 
  errCode = cudaMemcpy(tmp, dev_tmp, sizeof(unsigned int), cudaMemcpyDeviceToHost);
  if(errCode != cudaSuccess) 
  {
    cout << "\nError: getting tmp result form GPU error with code " << errCode << endl; 
  }
  // printf("\ntmp (changed to 69 on GPU): %d\n", *tmp);
  cudaFree(dev_tmp);
  return;
}