// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

#include <omp.h>
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

enum { SIZE = 50 };
int currentRow = 0;
pthread_mutex_t mutex;
double mat[SIZE][SIZE], tstart, thread1EndTime, thread2EndTime;

void *do_work(void *args);
double do_crazy_computation(int x, int y);

int main(int argc, char **argv)
{
  pthread_t thread[2];
  pthread_mutex_init(&mutex, NULL);
  int tid[2] = {0, 1};

  double tstart = omp_get_wtime();

  for (int t = 0; t < 2; t++) 
   {
    if (pthread_create(&thread[t], NULL, do_work, (void *)&tid[t])) 
    {
      fprintf(stderr, "Error while creating thread #%d!\n",t);
      exit(1);
    }
  }
  for (int t = 0; t < 2; t++) 
  {
    if (pthread_join(thread[t], NULL)) 
    {
      fprintf(stderr, "Error while joining the threads!\n");
      exit(1);
    }  
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

  pthread_mutex_destroy(&mutex);

  exit(0);
}

double do_crazy_computation(int x, int y)
{
  int iter;
  double value = 0.0;
  for (iter = 0; iter < 5 * x * x * x + 1 + y * y * y + 1; iter++)
  {
    value += (cos(x * value) + sin(y * value));
  }
  return value;
}

void *do_work(void *arg)
{
  int localRow = -1;
  int *tidPointer = (int *)arg;
  int tid = *tidPointer;

  while (currentRow < SIZE)
  {
    pthread_mutex_lock(&mutex);
    if (currentRow > SIZE)
    { return NULL; }
    localRow = currentRow;
    currentRow += 1;
    pthread_mutex_unlock(&mutex);

    for (int j = 0; j < SIZE; j++)
    { 
      mat[localRow][j] = do_crazy_computation(localRow, j);
      fprintf(stderr, ".");
    }

    if(!tid) // tid == 0, first thread
    { thread1EndTime = omp_get_wtime(); }
    else     // tid == 1, second thread
    { thread2EndTime = omp_get_wtime(); }
  }
  return NULL;
}