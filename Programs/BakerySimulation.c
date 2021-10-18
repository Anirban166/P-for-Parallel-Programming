// Author: Anirban166/Anirban Chetia
// E-mail: bloodraven166@gmail.com/ac4743@nau.edu

// Header files:
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

// Function declarations/prototypes:
void *do_work(void* arg);
void random_sleep(double a, double b);
double getDuration(struct timeval start, struct timeval stop);

// Fixed constants:
enum 
{ 
  NUM_ITERATIONS = 10,
  NUM_LEFT_OVEN_MITTS = 3,
  NUM_RIGHT_OVEN_MITTS = 3,
};

// Arguments struct:
struct arguments
{
  pthread_mutex_t *leftMittLock;                   // Lock for left mitt
  pthread_mutex_t *rightMittLock;                  // lock for right mitt
  pthread_cond_t *putLeftMittDown;                 // Condition variable for the status of a left mitt
  pthread_cond_t *putRightMittDown;                // Condition variable for the status of a right mitt
  int *availableLeftMitts;                         // Integer to keep track of the number of left mitts available
  int *availableRightMitts;                        // Integer to keep track of the number of right mitts available
  int id;                                          // Integer storing the baker ID (for a type of baker)
  char *bakerTypeName;                             // String indicating the type of baker, to be used in print statements
  enum bakerType{left, right, cautious}bakerType;  // Enum variable indicating the type of baker (left/right/cautious)
  double currentWaitTime;                          // Double storing the wait time for an iteration (for the running thread)
  double *leftHandedBakerWaitTime;                 // Double storing the total wait time for the left handed bakers
  double *rightHandedBakerWaitTime;                // Double storing the total wait time for the right handed bakers
  double *cautiousBakerWaitTime;                   // Double storing the total wait time for the cautious bakers
};

// Helper function to sleep a random number of microseconds, picked between two bounds: (provided in seconds)
void random_sleep(double lbound_sec, double ubound_sec) 
{
   int num_usec;
   num_usec = lbound_sec * 1000000 + (int)((ubound_sec - lbound_sec) * 1000000 * ((double)(rand()) / RAND_MAX)); 
   usleep(num_usec);
   return; 
}

// Helper function to get the elapsed time/duration in seconds and microseconds: (precision: 6 units)
double getDuration(struct timeval start, struct timeval stop)
{
  return (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
}

// Prime function which performs the entire baking cycle for the number of times required: (10 here)
void *do_work(void *arg) 
{
  struct arguments *threadArguments = (struct arguments *)arg;
  int *availableLeftMitts = threadArguments->availableLeftMitts;
  int *availableRightMitts = threadArguments->availableRightMitts;  
  pthread_mutex_t *leftMittLock = threadArguments->leftMittLock;
  pthread_mutex_t *rightMittLock = threadArguments->rightMittLock;
  pthread_cond_t *putLeftMittDown = threadArguments->putLeftMittDown;     
  pthread_cond_t *putRightMittDown = threadArguments->putRightMittDown;     
  struct timeval start_t, end_t;
  double diff_t, currentDuration;
  
  for (int i = 0; i < NUM_ITERATIONS; i++) 
  {
    // A baker prepares the cookies:     
    printf("[%s %d] is working...\n", threadArguments->bakerTypeName, threadArguments->id);
    // 1) A Baker works (i.e., he/she prepares dough, cuts cookies and puts them on a baking sheet)
    random_sleep(0.2, 0.5);
    
    // 2) A baker gets oven mitts from the oven rack:
    // Getting a left mitt for a left-handed/cautious baker:
    if(threadArguments->bakerType == left || threadArguments->bakerType == cautious)
    { 
      printf("[%s %d] wants a left-handed mitt...\n", threadArguments->bakerTypeName, threadArguments->id);
      pthread_mutex_lock(leftMittLock);      
      gettimeofday(&start_t, NULL);
      while(*availableLeftMitts == 0) 
      {
        pthread_cond_wait(putLeftMittDown, leftMittLock);
      }
      gettimeofday(&end_t, NULL);
      currentDuration = getDuration(start_t, end_t);
      threadArguments->currentWaitTime += currentDuration;
      if (threadArguments->bakerType == left)
      {
        *(threadArguments->leftHandedBakerWaitTime) += currentDuration;
      }
      else
      {
        *(threadArguments->cautiousBakerWaitTime) += currentDuration;
      }
      // Note for future self: DO NOT USE --/++ -> more mitts will be taken.
      printf("[%s %d] has got a left-handed mitt...\n", threadArguments->bakerTypeName, threadArguments->id);      
      *availableLeftMitts -= 1;
      pthread_mutex_unlock(leftMittLock);
    }
    // Getting a right mitt for a right-handed/cautious baker:
    if(threadArguments->bakerType == right || threadArguments->bakerType == cautious)
    {
      printf("[%s %d] wants a right-handed mitt...\n", threadArguments->bakerTypeName, threadArguments->id);
      pthread_mutex_lock(rightMittLock);    
      gettimeofday(&start_t, NULL);
      while(*availableRightMitts == 0) 
      {
        pthread_cond_wait(putRightMittDown, rightMittLock);
      }
      gettimeofday(&end_t, NULL);
      currentDuration = getDuration(start_t, end_t);
      threadArguments->currentWaitTime += currentDuration;
      if (threadArguments->bakerType == right)
      {
        *(threadArguments->rightHandedBakerWaitTime) += currentDuration;
      }
      else
      {
        *(threadArguments->cautiousBakerWaitTime) += currentDuration;
      }
      printf("[%s %d] has got a right-handed mitt...\n", threadArguments->bakerTypeName, threadArguments->id);
      *availableRightMitts -= 1;
      pthread_mutex_unlock(rightMittLock);
    }

    // 3) A baker puts cookies in the oven and waits for them to be baked:
    printf("[%s %d] has put cookies in the oven and is waiting...\n", threadArguments->bakerTypeName, threadArguments->id);
    // 4) Sleeping to simulate the cookies being baked:
    random_sleep(0.2, 0.5);
    // 5) A baker removes cookies from the oven:
    printf("[%s %d] has taken cookies out of the oven...\n", threadArguments->bakerTypeName, threadArguments->id);
    
    // 6) A baker puts back the mitts on the rack:
    // left or cautious:
    if(threadArguments->bakerType == left || threadArguments->bakerType == cautious)
    {
      pthread_mutex_lock(leftMittLock);
      *availableLeftMitts += 1;      
      printf("[%s %d] has put back a left-handed mitt... \n", threadArguments->bakerTypeName, threadArguments->id);
      pthread_cond_signal(putLeftMittDown);
      pthread_mutex_unlock(leftMittLock);
    }
    // right or cautious:
    if(threadArguments->bakerType == right || threadArguments->bakerType == cautious)
    {
      pthread_mutex_lock(rightMittLock);
      *availableRightMitts += 1;      
      printf("[%s %d] has put back a right-handed mitt... \n", threadArguments->bakerTypeName, threadArguments->id);
      pthread_cond_signal(putRightMittDown);
      pthread_mutex_unlock(rightMittLock);
    }

    // 7) A baker reports its waiting time before leaving:
    fprintf(stderr, "[%s %d] WAIT-TIME = %f....\n", threadArguments->bakerTypeName, threadArguments->id, threadArguments->currentWaitTime);
  // End for loop (end of ten baking cycles)
  }
}

int main(int argc, char **argv) 
{
  int num_left_handed_bakers;
  int num_right_handed_bakers;
  int num_cautious_bakers;
  int seed;

  // Process command-line arguments:
  if (argc != 5) 
  {
    fprintf(stderr,"Usage: %s <# left-handed bakers> <# right-handed bakers> <# cautious bakers> <seed>\n",argv[0]);
    exit(1);
  }
  if ((sscanf(argv[1],"%d",&num_left_handed_bakers) != 1) || 
      (sscanf(argv[2],"%d",&num_right_handed_bakers) != 1) || 
      (sscanf(argv[3],"%d",&num_cautious_bakers) != 1) || 
      (sscanf(argv[4],"%d",&seed) != 1) ||
      (num_left_handed_bakers < 1) ||
      (num_right_handed_bakers < 1) ||
      (num_cautious_bakers < 1) ||
      (seed < 0)) 
  {
    fprintf(stderr,"Invalid command-line arguments... Aborting\n");
    exit(1);
  }
  
  // Seed the rng:
  srand(seed);

  // Creating the different types of bakers:
  pthread_t leftHandedBakers[num_left_handed_bakers];
  pthread_t rightHandedBakers[num_right_handed_bakers];
  pthread_t cautiousBakers[num_cautious_bakers];

  // Initializing two mutex locks and two condition variables for the left and right oven mitts:
  pthread_mutex_t leftMittLock, rightMittLock;
  pthread_cond_t putLeftMittDown, putRightMittDown;
  pthread_mutex_init(&leftMittLock, NULL);
  pthread_mutex_init(&rightMittLock, NULL);
  pthread_cond_init(&putLeftMittDown, NULL);
  pthread_cond_init(&putRightMittDown, NULL);

  // Initializing the variables accounting for the number of left and right mitts: (both 3 here)
  int availableLeftMitts = NUM_LEFT_OVEN_MITTS, availableRightMitts = NUM_RIGHT_OVEN_MITTS;
  // Initializing the variable that holds the wait time for a baker in one baking cycle:
  double currentWaitTime = 0;
  // Initializing the variables that hold the total wait times for each type of baker:
  double leftHandedBakerWaitTime = 0, rightHandedBakerWaitTime = 0, cautiousBakerWaitTime = 0;

  // Initializing the arguments struct and creating arguments for the three types of bakers:
  struct arguments leftHandedBakerArguments[num_left_handed_bakers];
  struct arguments rightHandedBakerArguments[num_right_handed_bakers];
  struct arguments cautiousBakerArguments[num_cautious_bakers];
  for (int t = 0; t < num_left_handed_bakers; t++)
  {
    leftHandedBakerArguments[t].id = t;
    leftHandedBakerArguments[t].leftMittLock = &leftMittLock;  
    leftHandedBakerArguments[t].rightMittLock = &rightMittLock;
    leftHandedBakerArguments[t].putLeftMittDown = &putLeftMittDown;
    leftHandedBakerArguments[t].putRightMittDown = &putRightMittDown;
    leftHandedBakerArguments[t].availableLeftMitts = &availableLeftMitts;
    leftHandedBakerArguments[t].availableRightMitts = &availableRightMitts;
    leftHandedBakerArguments[t].bakerType = left;
    leftHandedBakerArguments[t].bakerTypeName = "Left-handed baker";
    leftHandedBakerArguments[t].currentWaitTime = currentWaitTime;    
    leftHandedBakerArguments[t].leftHandedBakerWaitTime = &leftHandedBakerWaitTime;
    leftHandedBakerArguments[t].rightHandedBakerWaitTime = &rightHandedBakerWaitTime;
    leftHandedBakerArguments[t].cautiousBakerWaitTime = &cautiousBakerWaitTime;
    if (pthread_create(&leftHandedBakers[t], NULL, do_work, (void *)&(leftHandedBakerArguments[t]))) 
    {
      fprintf(stderr, "Error while creating thread #%d!\n",t);
      exit(1);
    }
  }
  for (int t = 0; t < num_right_handed_bakers; t++)
  {
    rightHandedBakerArguments[t].id = t;
    rightHandedBakerArguments[t].leftMittLock = &leftMittLock;
    rightHandedBakerArguments[t].rightMittLock = &rightMittLock;    
    rightHandedBakerArguments[t].putLeftMittDown = &putLeftMittDown;
    rightHandedBakerArguments[t].putRightMittDown = &putRightMittDown;
    rightHandedBakerArguments[t].availableLeftMitts = &availableLeftMitts;
    rightHandedBakerArguments[t].availableRightMitts = &availableRightMitts;
    rightHandedBakerArguments[t].bakerType = right;
    rightHandedBakerArguments[t].bakerTypeName = "Right-handed baker";
    rightHandedBakerArguments[t].currentWaitTime = currentWaitTime;    
    rightHandedBakerArguments[t].leftHandedBakerWaitTime = &leftHandedBakerWaitTime;
    rightHandedBakerArguments[t].rightHandedBakerWaitTime = &rightHandedBakerWaitTime;
    rightHandedBakerArguments[t].cautiousBakerWaitTime = &cautiousBakerWaitTime;    
    if (pthread_create(&rightHandedBakers[t], NULL, do_work, (void *)&(rightHandedBakerArguments[t]))) 
    {
      fprintf(stderr, "Error while creating thread #%d!\n",t);
      exit(1);
    }
  }  
  for (int t = 0; t < num_cautious_bakers; t++)
  {
    cautiousBakerArguments[t].id = t;
    cautiousBakerArguments[t].leftMittLock = &leftMittLock;
    cautiousBakerArguments[t].rightMittLock = &rightMittLock;    
    cautiousBakerArguments[t].putLeftMittDown = &putLeftMittDown;
    cautiousBakerArguments[t].putRightMittDown = &putRightMittDown;
    cautiousBakerArguments[t].availableLeftMitts = &availableLeftMitts;
    cautiousBakerArguments[t].availableRightMitts = &availableRightMitts;
    cautiousBakerArguments[t].bakerType = cautious;
    cautiousBakerArguments[t].bakerTypeName = "Cautious baker";
    cautiousBakerArguments[t].currentWaitTime = currentWaitTime;    
    cautiousBakerArguments[t].leftHandedBakerWaitTime = &leftHandedBakerWaitTime;
    cautiousBakerArguments[t].rightHandedBakerWaitTime = &rightHandedBakerWaitTime;
    cautiousBakerArguments[t].cautiousBakerWaitTime = &cautiousBakerWaitTime;    
    if (pthread_create(&cautiousBakers[t], NULL, do_work, (void *)&(cautiousBakerArguments[t]))) 
    {
      fprintf(stderr, "Error while creating thread #%d!\n",t);
      exit(1);
    }
  }  

  // Joining the baker threads:
  for (int t = 0; t < num_left_handed_bakers; t++) 
  {
    if (pthread_join(leftHandedBakers[t], NULL)) 
    {
      fprintf(stderr, "Error while joining the threads!\n");
      exit(1);
    }  
  }
  for (int t = 0; t < num_right_handed_bakers; t++) 
  {
    if (pthread_join(rightHandedBakers[t], NULL)) 
    {
      fprintf(stderr, "Error while joining the threads!\n");
      exit(1);
    }  
  }
  for (int t = 0; t < num_cautious_bakers; t++) 
  {
    if (pthread_join(cautiousBakers[t], NULL)) 
    {
      fprintf(stderr, "Error while joining the threads!\n");
      exit(1);
    }  
  }

  // Printing the average wait times for each type of baker:
  fprintf(stderr, "Average wait times:\n");
  fprintf(stderr, "Left-handed bakers average wait time: %f/%d = %f\n", leftHandedBakerWaitTime, num_left_handed_bakers, (leftHandedBakerWaitTime / (double)num_left_handed_bakers));
  fprintf(stderr, "Right-handed bakers average wait time: %f/%d = %f\n", rightHandedBakerWaitTime, num_right_handed_bakers, (rightHandedBakerWaitTime / (double)num_right_handed_bakers));
  fprintf(stderr, "Cautious bakers average wait time: %f/%d = %f\n", cautiousBakerWaitTime, num_cautious_bakers, (cautiousBakerWaitTime / (double)num_cautious_bakers));

  // Unitializing both the locks and condition variables:
  pthread_mutex_destroy(&leftMittLock);
  pthread_mutex_destroy(&rightMittLock);
  pthread_cond_destroy(&putLeftMittDown);
  pthread_cond_destroy(&putRightMittDown);
  
  // Exiting the program (without any errors):
  exit(0);
}