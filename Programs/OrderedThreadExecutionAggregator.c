// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

// Header files:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Setting the desired sum plus the number of threads working on to achieve that aggregate value:
static const int requiredSum = 990;
static const int threadCount = 10;

// Function declarations/prototypes:
void *do_work(void *arg);

// Global variable that is updated by all threads (in order):
unsigned int total = 0;

// Arguments struct:
struct arguments 
{ 
  int value;
  int *nextCount;
  pthread_mutex_t *mutex;
  pthread_cond_t *incrementInProgress;
};

// Required function:
void *do_work(void *arg)
{
  struct arguments *threadArguments = (struct arguments *)arg;
  int *nextCount = threadArguments->nextCount;

  pthread_mutex_lock(threadArguments->mutex);
  // Looping till total is not equal to 900: (will exit when it is, given by an internal check)
  while (total != requiredSum)
  {
    // Waiting until it's this thread's turn to run:
    while (*nextCount != threadArguments->value) 
    {
      pthread_cond_wait(threadArguments->incrementInProgress, threadArguments->mutex);
    }
    if (total == requiredSum) 
    {
    // Exiting and telling the next thread (that was waiting) to exit as well:
      (*nextCount)++;
      pthread_cond_broadcast(threadArguments->incrementInProgress);
      pthread_mutex_unlock(threadArguments->mutex);
      return NULL;
    }
    // If we're here, we know that *nextCount == id of the thread (threadArguments->value)
    // Otherwise, the thread will wait in the above loop.
    total += threadArguments->value;
    printf("my num: %d, total: %d\n", threadArguments->value, total);
    if (total == requiredSum)
    {
      printf("Total: %d\n", total);  
    }
    // If the thread with value 9 (last in an ordered group of 10) is here, reset the counter, 
    // else increment it:
    *nextCount = (*nextCount == 9) ? 0 : ++*nextCount;
    // Send a signal to all waiting threads to have them check the condition above:
    pthread_cond_broadcast(threadArguments->incrementInProgress);
  }
  pthread_mutex_unlock(threadArguments->mutex);
  return NULL;
}

int main(int argc, char *argv) 
{
  // Initializing counter variables and declaring the array of threads:
  int total = 0, nextCount = 0;
  pthread_t thread[threadCount];	

  // Initializing the mutex lock and the condition variable to be used:
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_t incrementInProgress;
  pthread_cond_init(&incrementInProgress, NULL);
  
  // Initializing the arguments struct and creating arguments for the ten threads:
  struct arguments *threadArguments[threadCount];
  for (int i = 0; i < threadCount; i++)
  {
    threadArguments[i] = (struct arguments *)calloc(1, sizeof(struct arguments));
    threadArguments[i]->value = i; // thread[0]->0, thread[1]->1 ... thread[9]->9
    threadArguments[i]->nextCount = &nextCount;
    threadArguments[i]->mutex = &mutex;
    threadArguments[i]->incrementInProgress = &incrementInProgress;
  }

  // Creating the ten threads: (with the arguments passed onto the do_work function)
  for (int t = 0; t < threadCount; t++) 
  {
    if (pthread_create(&thread[t], NULL, do_work, (void *)threadArguments[t])) 
    {
      fprintf(stderr, "Error while creating thread #%d!\n",t);
      exit(1);
    }
  }

  // Joining them:
  for (int t = 0; t < threadCount; t++) 
  {
    if (pthread_join(thread[t], NULL)) 
    {
      fprintf(stderr, "Error while joining the threads!\n");
      exit(1);
    }  
  }

  // Unitializing the mutex and condition variables:
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&incrementInProgress);

  // Exiting the program (without any errors):
  exit(0); 
}
