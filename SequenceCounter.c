// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

// Header files:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

// Setting the number of threads plus the required number of correct sequences:
static const int threadCount = 3;
static const int requiredNumberOfCorrectSequences = 10;

// Function declarations/prototypes:
void *do_work(void *arg);
bool createdSequence(int buffer[]);
void clearBuffer(int *bufferPointer, int bufferSize);

// Arguments struct:
struct arguments
{
  pthread_mutex_t *mutex;     // Lock for mutual exclusion
  int value;                  // Variable containing an integer value (one for each thread)
  int *buffer;                // Buffer to store values (3)
  int *bufferIndex;           // Variable to store the buffer/array index
  int *correctSequencesCount; // Variable to count the no. of '123' sequences made
  int *totalSequencesCount;   // Variable to count the total no. of attempts made by the threads to make the sequence '123'
};

// Function to clear the buffer:
void clearBuffer(int *bufferPointer, int bufferSize)
{
  for (int i = 0; i < bufferSize; i++)
  {
    *bufferPointer = 0;
    bufferPointer++;
  }
}

// Function to check if the desired sequence (123) is created:
bool createdSequence(int buffer[])
{
  return (buffer[0] == 1 && buffer[1] == 2 && buffer[2] == 3);
}

// Required function:
void *do_work(void *arg)
{
  struct arguments *threadArguments = (struct arguments *)arg;
  pthread_mutex_t *mutex = threadArguments->mutex;
  int *correctSequencesCount = threadArguments->correctSequencesCount;
  int *totalSequencesCount = threadArguments->totalSequencesCount;
  int *indexPointer = NULL;
  
  // Looping until the desired number of correct sequences (10) is obtained:
  while (*correctSequencesCount < requiredNumberOfCorrectSequences)
  {
    // Critical section start: (time to use mutex!)
    pthread_mutex_lock(mutex);
    indexPointer = threadArguments->bufferIndex;
    printf("My id: %d\n", threadArguments->value);
    threadArguments->buffer[*indexPointer] = threadArguments->value;
    // If we reach the third element (starting from index 0), count it as an attempt to make the sequence:
    if (*indexPointer == 2)
    {
      *totalSequencesCount += 1;
       // If the desired sequence (123) is created, print it and increment the counter accounting for that: 
       if (createdSequence(threadArguments->buffer))
       {
         printf("123\n");
         *correctSequencesCount += 1;
         // If ten desired sequences (123) are completed, we are done and so I print the counts:
         if (*correctSequencesCount == requiredNumberOfCorrectSequences)
         {
           printf("Total sequences generated: %d\n", *(threadArguments->totalSequencesCount));
           printf("Number of correct sequences: %d\n", *(threadArguments->correctSequencesCount));
         }
       }
       // Clear the buffer and reset it's index pointer (back to 0):
       clearBuffer(threadArguments->buffer, 3);
       *indexPointer = 0;
    }
    else
    {
      *indexPointer += 1;
    }
    pthread_mutex_unlock(mutex);
    // Slumber call: (after critical section!)
    usleep(500000);
    }
  return NULL;
}

int main(int argc, char *argv)
{
  // Declaring the array of pthreads (3) and initializing required variables to zero:
  pthread_t thread[threadCount]; 
  int buffer[3] = {0, 0, 0};   
  int bufferIndex = 0;          
  int correctSequencesCount = 0;
  int totalSequencesCount = 0;   

  // Initializing a mutex lock:
  pthread_mutex_t bufferLock;
  pthread_mutex_init(&bufferLock, NULL);

  // Initializing the arguments struct and creating arguments for the threads:
  struct arguments *threadArguments[threadCount];
  for (int i = 0; i < threadCount; i++)
  {
    threadArguments[i] = (struct arguments *)calloc(1, sizeof(struct arguments));
    threadArguments[i]->mutex = &bufferLock;
    threadArguments[i]->value = i + 1; // thread[0]->1, thread[1]->2, thread[2]->3
    threadArguments[i]->buffer = buffer;
    threadArguments[i]->bufferIndex = &bufferIndex;
    threadArguments[i]->correctSequencesCount = &correctSequencesCount;
    threadArguments[i]->totalSequencesCount = &totalSequencesCount;
  }
  
  // Creating the three threads: (with arguments passed on to the do_work function)
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
  
  // Unitializing the mutex lock:
  pthread_mutex_destroy(&bufferLock);
    
  // Exiting the program (without any errors):
  exit(0); 
}
