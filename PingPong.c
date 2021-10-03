// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

// Header files:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Function declarations/prototypes:
void *increment_work(void *arg);
void *decrement_work(void *arg);

// Arguments struct:
struct arguments 
{
  int *count;
  pthread_mutex_t *mutex;
  pthread_cond_t *count_zero;
  pthread_cond_t *count_not_zero;
};

// Increment function:
void *increment_work(void *arg)
{
  struct arguments *threadArguments = (struct arguments*) arg;
  int rounds = 0;
  pthread_mutex_lock(threadArguments->mutex);
  // Do one round of increments: (count value 0 -> 10)
  for(int i = 0; i < 10; i++)
  {
    (*(threadArguments->count)) ++;
    printf("Count is now (inc fn): %d\n", *(threadArguments->count));
  }

  pthread_cond_signal(threadArguments->count_not_zero);
  pthread_mutex_unlock(threadArguments->mutex);
   
  // Do two rounds of increments:
  while(rounds < 2) 
  {
    pthread_mutex_lock(threadArguments->mutex);
    // Wait for the count value to be zero:
    while((*(threadArguments->count)) != 0)
    {
      pthread_cond_wait(threadArguments->count_zero, threadArguments->mutex);
    }  
    // Increment count:  
    for(int i = 0; i < 10; i++)
    {
      (*(threadArguments->count))++;
      printf("Count is now (inc fn): %d\n", *(threadArguments->count));
    }
    pthread_mutex_unlock(threadArguments->mutex);
    // Signal the decrementing thread that count is no longer zero (10):
    pthread_cond_signal(threadArguments->count_not_zero);
    rounds++;
  }
  return NULL;
}

// Decrement function:
void *decrement_work(void *arg)
{
  struct arguments *threadArguments = (struct arguments*) arg;
  int rounds = 0;
  // Do two rounds of decrements:
  while(rounds < 2) 
  {
    pthread_mutex_lock(threadArguments->mutex);
    // Wait till the incrementing thread increments the count value till 10:
    while ((*(threadArguments->count)) != 10) 
    {
      pthread_cond_wait(threadArguments->count_not_zero, threadArguments->mutex);
    }
    // Decrement count:
    for(int i = 0; i < 10; i++)
    {
      (*(threadArguments->count)) --;
      printf("Count is now (dec fn): %d\n", *(threadArguments->count));
    }
    // Send a signal to the incrementing thread indicating that the count value is 0:
    pthread_cond_signal(threadArguments->count_zero);
    pthread_mutex_unlock(threadArguments->mutex);
    rounds++;
  }
  return NULL;
}

int main(int argc, char *argv) 
{
  // Initializing the count value (0) and declaring the threads which will update it:
  int count = 0;
  pthread_t incrementer, decrementer;	

  // Initializing the mutex and condition variables:
  pthread_mutex_t mutex;
  pthread_cond_t count_zero;
  pthread_cond_t count_not_zero;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&count_zero, NULL);
  pthread_cond_init(&count_not_zero, NULL);
  
  // Initializing the arguments struct and creating arguments for the two threads:
  struct arguments threadArguments[2];
  for (int i = 0; i < 2; i++)
  { // Shared count value between the threads:
    threadArguments[i].count = &count;
    // Shared lock between the threads:
    threadArguments[i].mutex = &mutex; 
    // Shared condition variable indicating that the count value is zero:
    threadArguments[i].count_zero = &count_zero;
    // Shared condition variable indicating that the count value is not zero:
    threadArguments[i].count_not_zero = &count_not_zero;
  }

  // Creating the count value incrementing thread:
  if (pthread_create(&incrementer, NULL, increment_work, (void *)&(threadArguments[0]))) {
    fprintf(stderr,"Error while creating the incrementer thread!\n");
    exit(1);
  }
  // Create the count value decrementing thread:
  if (pthread_create(&decrementer, NULL, decrement_work, (void *)&(threadArguments[1]))) {
    fprintf(stderr,"Error while creating decrementer thread!\n");
    exit(1);
  }
  
  // Joining them:
  if (pthread_join(incrementer, NULL)) 
  {
    fprintf(stderr,"Error while joining the threads!\n");
    exit(1);
  }  
  if (pthread_join(decrementer, NULL)) 
  {
    fprintf(stderr,"Error while joining the threads!\n");
    exit(1);
  }  
  
  // Unitializing the mutex and condition variables:
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&count_zero);
  pthread_cond_destroy(&count_not_zero);  
  
  // Exiting the program (without any errors):
  exit(0); 
}
