# P-for-Parallel-Programming
Contains **some** parallel programs that I wrote while experimenting with the POSIX threads api.

> Emphasizing on 'some', given that there are sequential programs here as well, including ones that are worser than their normal sequential run (equivalent with no threads), due to factors such as locking overhead. 

<details>
<summary> Sequence Generation </summary>

- Problem/Question:
  
Having multiple threads call a function like do_work(...) will have a non-deterministic execution. Write a program with 3 threads that call a function called do_work. Each thread will be responsible for generating a number and appending it to a buffer. Thread 1 generates number 1, thread 2 generates number 2, and thread 3 generates number 3. These numbers assigned to the threads are passed in as arguments. Each thread will store its value in a shared buffer of integers having a size of 3 elements called "buffer". When the third element is added to the buffer by either thread 1, 2 or 3, it checks to see if the sequence is "123". If not, it clears the buffer and the threads try to generate the sequence again. Once the total number of sequences of "123" reach 10, the threads should exit the function and join with the main thread. Each time you generate "123", it should be printed to the screen. You should also print out the total number of tries it took to print "123". For example, keep track of the total number of other sequences generated (including 123), as well: 321, 213, etc. You must use the usleep(500000) function once at each iteration (after each time a thread updates the buffer, but not in a critical section). Also, each time a thread adds its element to the buffer, it should print out it's corresponding number. Provided below is an example of the expected output at the end of the program's execution: (ensure that your program produces the exact same output formatting) <br>
... <br>
My id: 1 <br>
My id: 3 <br>
My id: 2 <br>
My id: 1 <br>
My id: 2 <br>
My id: 3 <br>
123 <br>
Total sequences generated: 45 <br>
Number of correct sequences: 10 <br>

- Python script to test this program with: [test_SequenceOrder.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_SequenceCounter.py)

- My solution: [SequenceCounter.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/SequenceCounter.c)
  
- Code explanation:

The threads here (3) have six arguments (from a struct) - an integer value (enacting like a thread ID, with the first thread/thread[0] being assigned 1, the second being assigned 2, and the third one being assigned 3), a buffer (with a capacity of 3) to store those values, a pointer for use in pointing to the buffer index, and finally the counting variables - one accounting for the correct number of ‘123’ sequences, and the other accounting for all the sequences (buffer output or prints of 3 elements/integers), irrespective of whether they form our desired sequence (123) or not.

I created two helper functions - clearBuffer to literally clear the buffer (setting its elements to 0 - it takes the pointer to the buffer index and the buffer size as its arguments) and createdSequence to check (yup, it is a boolean returning true/false - it takes the buffer as an argument) if the buffer’s elements are ‘1’, ‘2’ and ‘3’, in respective order for indices 0, 1 and 2.

Now inside my do_work function, I create a pointer named threadArguments to point to the received struct (passed on via main while creating the threads, after the threads get assigned their struct arguments therein) and then I create some variables to contain some of those arguments for use inside the function. I then created a while loop which runs until our desired number of sequences (ten) have been created (runs ten times from 0 to 9, or from 0 to < 10). Since I am now going to deal with reading, incrementing and writing the value of the count variables and operations (may it be just printing) which should be restricted to a thread (in order for the variable-access and updates to be atomic, or the operations to be thread safe, so as to avoid race conditions), I demark this as the start of the critical section and so I lock it with my mutex lock (one of the arguments from my struct). I then print the value that the thread which runs that portion of code contains (which again, is one of the arguments from my struct), in the format as specified in the requirement, and the one which would satisfy the python test script. I then set my index pointer (made sure to avoid segfaults) to the bufferIndex argument and then I emplace it to point at the index of my buffer, and that as a whole (buffer at that position/index) to add and contain the value that the thread running the do_work function has. If the index is two, it means we reached the third element in the buffer and it will count as a sequence (irrespective of correctness), so I increment the counter accounting for the total number of sequences. Then I use an if statement with the conditional being the boolean returned from my function createdSequence, which checks if the correct sequence is being made. If it is, ‘123’ is printed and the counter accounting for the total number of correct sequences is incremented. If that counter reaches our desired number (10), it executes the final print statements with both the counter values to give the statistics. Then, I clear my buffer (using my clearBuffer function) because I reached its capacity (3) with the third addition, and I reset the index pointer (back to 0). This is done for every third element, or for every third thread that goes through do_work(). For the rest (first and second elements in order), the index pointer has to be simply incremented. With the critical section being finally over, I unlock the mutex and make a usleep(500000) call as specified. (with the resultant output of flowing numbers moving at a pace at which we can notice and analyze) 

Coming to the main function, it first simply declares the array of pthreads, initializes the buffer elements, the buffer index and the counting variables to zero, creates and initializes a mutex lock, and then it creates and initializes the arguments struct (array of 3) with the six arguments. (ones mentioned at the beginning). Next up, these are then subsequently assigned to the threads that are created using pthread_create() and then joined using pthread_join() in separate for-loops. Finally, the created mutex is uninitialized and the program exits successfully (without an error or with exit(0), given that it reaches that point).  
</details>

<details>
<summary> Sequence Competition </summary>

- Problem/Question:
  
There is a competition to generate the sequences faster between two sets of 3 threads (6 threads in total). This extends the question above, so reuse your code and output the same information. Another 3 threads will generate the values 4, 5, 6 just like in problem one. Whichever set of 3 threads generates 10 sequences first, either "123" or "456", wins. Furthermore, whichever set of 3 threads wins, must tell the other set of threads that they have won and stop their execution. Only 1 thread must tell the other set of threads. Make functions 'do_work' for the first 3 threads and 'do_work2' for the second set of 3 threads. Use two buffers, named "buffer1" and "buffer2" respectively. You should also print out the total number of tries it took to print "123" and "456", and the total number of correct sequences that each set of 3 threads generated. One should be
10 and the other should be less than 10. The set of 3 threads that won should print to the screen only once: "Team x won!", where x is either 1 or 2. You must use usleep(500000) for both sets of threads at each iteration (but do not sleep in a critical section). Provided below is an example of the expected output where team 2 wins: <br>
... <br>
My id: 5 <br>
My id: 3 <br>
123 <br>
My id: 6 <br>
456 <br>
Team 2 won! <br>
Total sequences generated team1: 51 <br>
Number of correct sequences team1: 7 <br>
Total sequences generated team2: 51 <br>
Number of correct sequences team2: 10 <br>

- Python script to test this program with: [test_SequenceCompetition.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_SequenceCompetition.py)

- My solution: [SequenceCompetition.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/SequenceCompetition.c)

- Code explanation:

My approach for solving this is largely similar to problem one, given how similar the requirements for each are. In most cases, there are twice the amount of variables I had to use here in comparison to problem one, such as for the counters accounting for the correct and total number of sequences - one each for each set of three threads, and since they have to be shared (explained below), they are (and have to be) declared separately in my struct.
For the buffers, the buffer index variables, and the locks, they are separate and specific to the set of threads and their respective do_work functions, and so they are two variables for one category (buffer/lock etc.) as declared and initialized within my main function, but they are not separate arguments in my struct since they do not have to be shared, and again, are specific to a set of threads only. There is a new variable named winCondition that I introduced in my struct, which is a boolean shared between both set of threads so as to obstruct the other set from continuing in its own do_work function (otherwise both set of threads will run for the required number of correct sequences, i.e. 10 times each!) if either of them have completed 10 correct (respective) sequences first.

Regarding my helper functions, clearBuffer stays the same, but createdSequence gets a separate argument (threadSet) in order to distinguish between the correct sequence (123 or 456) when used/called.

Now coming to our prime functions do_work and do_work2, (I am going to walk you through them together since they are essentially the same) I start by creating threadArguments (pointer to struct passed on from main for a thread) and assigning some of the arguments it contains to referenced copies of some shared variables (the counters and the win-indicating flag) and some variables specific to a set of threads (the index pointer and the mutex lock) for use inside the critical section. Proceeding next to the while-loop, I use a logical AND with a negation of the winCondition variable (since it is initially set to false in main) which is coupled with the usual ‘until the number of correct sequences for this set of threads reaches the desired number of total correct sequences’, (which is the win condition) for the condition of the loop. In other words, I loop till I get ten correct sequences of ‘123’ from do_work(), or till I get ten correct sequences of ‘456’ from do_work2(), but whichever set of threads or function does it first, it will set winCondition to true which when negated and subjected to a logical AND with the while loop, will stop the threads from further execution in their respective functions, indicating that one team (or set of threads) won, and that its game over.

Thus, in order to follow this logic, I emplace an if-conditional which will break the loop and exit after unlocking the mutex (which is, of course, locked at the beginning for the for-loop since the critical section follows after, like in problem one). Again, rest of the loop’s logic is exactly the same as what I went with and explained in problem one, with the exception that the counter variables are separate and specific to the set of threads now and that after the increments are done, we need to print the total results in case a team has won or completed 10 correct sequences. For this, we’ll require all the four counters (two for each set of threads) and that is why they have to be shared in the first place. They are printed (along with the required structure of the print message, one which satisfies the python script) inside an if-statement with the condition being ‘number of correct sequences for this set of threads equal to the desired number of total correct sequences’ along with the winCondition variable being set/true. Then I unlock the mutex and make the call to usleep() to make it fair (equal number of microseconds allotted) and noticeable for both sets of threads when they are running, or in competition mode.

I’ve already explained most of the critical parts for the main function above, but I’ll go with it in short - it’s the same old (with respect to problem one) deal, i.e. I declare/create and initialize or assign values to the pthreads (also join them), the shared and ‘set-of-threads’-specific variables, and destroy the locks at the end after the work is done, and before the program exits.

Note that I have specified the number of threads and the number of correct sequences required at the beginning like in the first problem (also for problem number three, wherein the required sum and thread count is fixed at the starting, and can be changed appropriately for easily making the program flexible for other values), for convenience such as to quickly change the required number of correct sequences. (could have also done for the sequences, yes) But more changes have to be made (like extending/limiting the size of buffers appropriately, and deciding which team gets to have more number of threads for an odd total number of threads) if we were to change the thread values for problem one and two, and divide the teams in a fair manner. Note that this is just a thought for making the program more generalized, and the program as it is, was made with the intention to satisfy what’s required to solve problem number two.
</details>

<details>
<summary> Ordering Thread Execution </summary>

- Problem/Question:
  
Often it's useful to order thread execution. In this problem, we generate 10 threads - each is assigned a value 0 through 9, respectively (i.e., thread 0 is assigned value 0, thread 1 is assigned value 1, ..., and thread 9 is assigned value 9). Each thread calls a function do_work(). There is a global variable called "total". Each thread takes a turn adding its value to total, i.e., adding 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, and then adding 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 again, and so on. It must add the values in order, i.e., 5 cannot be added before 2 in a single "sequence". If you do this 22 times, the total value will be 990 ((0+1+2+3+4+5+6+7+8+9)*22=990). Write a program where each thread adds its value to total, over and over, which stops adding when total is equal to 990. After the threads join, the value of "total" should be output to the screen. You may not put your threads to sleep during the execution of the program, although feel free to use usleep() when testing and debugging. Every time a thread updates total it should output it's assigned number and the updated value of total. Provided below is the example of the expected output at the beginning and at the end: <br>
my num: 0, total: 0 <br>
my num: 1, total: 1 <br>
my num: 2, total: 3 <br>
my num: 3, total: 6 <br>
my num: 4, total: 10 <br>
... <br>
my num: 6, total: 966 <br>
my num: 7, total: 973 <br>
my num: 8, total: 981 <br>
my num: 9, total: 990 <br>
Total: 990 <br>

- Python script to test this program with: [test_OrderedThreadExecutionAggregator.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_OrderedThreadExecutionAggregator.py)

- My solution: [OrderedThreadExecutionAggregator.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/OrderedThreadExecutionAggregator.c)

- Code explanation:

For this problem, we require four variables in our arguments struct to be passed onto the ten threads to be created - an integer value (for each thread), a pointer to an integer value which will store the count which would indicate which thread should go next in an iterative threadstep, a mutex lock, and finally, a condition variable (unlike the past two problems). 

I made the counter’s (total) value global as specified in the pdf for this assignment, and I specified the thread count and required end-value of total towards the start of the program code (right after the inclusion of the header files) for ease of modification, in case you might want to try other values for them.

Coming to the meat of the solution, or the do_work function, it first creates a pointer to the struct as usual and from there it extracts the nextCount variable as a referenced copy for use inside the function’s critical section. I then lock the mutex and proceed to create a while-loop which runs until we get total to our desired value (990). Inside it, I emplace another while-loop which checks if the current/running thread’s value argument (or the thread-ID if you prefer, since they are often used interchangeably) is not equal to the current value of nextCount. (For instance, initially only thread #0 will satisfy that condition, and then only thread #1 will, once the nextCount is incremented by 1) If it is not, it will have to wait for its turn, which is given by a pthread_cond_wait(), which takes my condition variable named incrementInProgress, and of course, the mutex lock (which is available from the arguments struct for the thread) since waiting would basically play with the lock (unlocking and checking if there is a signal, and again locking back if there is not).

Post that loop-based check, it means that it’s our desired thread’s turn to run.
I then check if the value for total is our required sum (990) again, since if that is true at this point, the running thread should break out of the function (or return NULL since its a void type one) and exit, after incrementing the nextCount value and signalling the other waiting threads (telling them to exit as well) and after unlocking the mutex. Note that this check is for threads which run after the last thread (thread #9 running for the 22nd time to be more specific) requiring to increment its value to total to make it a cumulative of 990. That is, it is for the ones who are left waiting and have to be all signalled (broadcasted, of course) to be called off-duty, since my goal is achieved at this point. The equivalent check for threads who still need to get their values accounted for to make the 990 is there below, which I am coming to in a moment. If we’re past the first check of (total == requiredSum) and still in the while-loop, it means that we haven’t reached our desired value for total and we should increment it by the value designated/assigned to the running thread. After incrementing, I then print that value to stdout, along with the value of total up so far in a print statement as required for this problem, and as would be satisfactory with the python test script.

Now comes the second (total == requiredSum) check, under which the total value is printed once and for all as the last line of output, which is meant to be executed by the last thread incrementing total to make it equal to the desired sum value (right before the check), which will be thread #9 here with our values, for the 22nd iteration, like I mentioned above. Following next is a ternary operator conditional I used (I felt it was better to avoid code bloating with the standard if-else, to use something more compact with no performance loss in modern compilers) which resets the value of nextCounter to 0 if thread number 9 is running the segment, else it increments it by 1.

Then I use a pthread_cond_broadcast() to signal all waiting threads to keep checking the condition(s) above and to avoid the program from resulting in a deadlock. After exiting the while loop, I finally unlock the mutex for a regular case.

The main function here is pretty standard again, involving initializations of the counter variables, the mutex lock and the condition variable, and creation of the threads, assignment of arguments to them and joining them with the main thread. At last, I uninitialize the lock and the condition variable.
</details>

<details>
<summary> Ping Pong </summary>

- Problem/Question:
  
Write a program using 2 threads, which call decrement_work() and increment_work(), respectively. Maintain a shared counter that is initialized to 0. The function increment_work() increments the counter by 1 at each loop iteration, and decrement_work() decrements the counter by 1 at each loop iteration. Increment the counter value from 0 to 10 in increment_work(). Once the counter hits 10, the thread should wait. Then the decrementing thread should decrement the counter until the value is 0. Then it signals the incrementing thread that the value is 0, and then the incrementing thread begins incrementing the counter. The decrementing thread waits until it is signaled that the value is 10 and so on. The total number of increments and decrements combined is 50, such that the final value is 10, (i.e., 30 total increments and 20 total decrements). Implement this using locks and condition variables. You may not put your threads to sleep during the execution of the program. Each time a thread modifies the counter, output it to the screen. Provided below is the example of the expected output at the end of the program: <br>
... <br>
Count is now (inc fn): 9 <br>
Count is now (inc fn): 10 <br>
Count is now (dec fn): 9 <br>
Count is now (dec fn): 8 <br>
Count is now (dec fn): 7 <br>
Count is now (dec fn): 6 <br>
Count is now (dec fn): 5 <br>
Count is now (dec fn): 4 <br>
Count is now (dec fn): 3 <br>
Count is now (dec fn): 2 <br> 
Count is now (dec fn): 1 <br>
Count is now (dec fn): 0 <br>
Count is now (inc fn): 1 <br>
Count is now (inc fn): 2 <br>
Count is now (inc fn): 3 <br>
Count is now (inc fn): 4 <br>
Count is now (inc fn): 5 <br>
Count is now (inc fn): 6 <br>
Count is now (inc fn): 7 <br>
Count is now (inc fn): 8 <br>
Count is now (inc fn): 9 <br>
Count is now (inc fn): 10 <br>

- Python script to test this program with: [test_PingPong.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_PingPong.py)

- My solution: [PingPong.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/PingPong.c)

- Code explanation:

For the last problem, we only require one counter variable (unlike the previous one where we required a global counter which would be incremented up until our desired value, and then we required another counter to keep track of the thread IDs), but we require two condition variables (or at least that’s what I think!) apart from a mutex for our arguments struct. These two condition variables that I used here are used to indicate the status of the shared counter variable (count) with relation to it being either zero or not. (and hence the condition variables follow my intuitive naming convention - countZero and countNotZero) 

Going by the problem’s specifications, there are two functions to create - one which is used by the thread which increments the shared counter, and one which is used by the thread which decrements it, namely increment_work and decrement_work (bearing with the itch to write every function name in camel case!), the design for both of which I made slightly different, being forced to while going by the requirements. To elaborate, we require the incrementing thread to run for more times (30) than the decrementing thread (20) since the end value for count should be 10 (30-20), so they can’t be made to follow the same number of iterative steps. This is the reason why I first increment the shared counter from 0 to 10 in increment_work under a for-loop, before proceeding to the while-loop with 2*10 iterations, (this part is similar to the one in decrement_work as well).

Starting with increment_work, I first create a pointer to the struct as in my problems before and then I create a referenced copy for the shared variable count (could have also done the same for the mutex and the condition variables, instead of prefixing ‘threadArguments->’, but thought to go with a change for problem three and four) for use inside the function’s critical section. Then I place a lock using the one from my arguments struct for the running thread as I am accessing and updating the count value next (reading, then incrementing and then writing its value, all of which need to be atomic). I do a round of ten increments of one, so as to get the ping-pong game started.  
Now since count is no longer zero (10), I use pthread_cond_signal to signal this to the decrementing thread (the only other thread) so that it can operate (otherwise it would keep waiting, as I’ll discuss that in a moment). I then unlock my mutex lock and proceed to my while-loop, which iterates for two times, and contains 10 increments (of one) each for count under a for-loop.
Right after I enter this loop, I put my mutex lock back on since we are reading and dealing with count again. I check if it's not equal to zero under a while-loop, and if that is the case, then it needs to wait for the decrementing thread to perform its decrements and update its value back to 0. A call to pthread_cond_wait with the countZero condition variable (plus the mutex) takes care of this. If it gets the signal from decrement_work, it will continue to move forward to the for-loop code segment which increments count back to 10. Then I unlock the mutex and send a signal back to decrement_work again, indicating that the count value is no longer 0 (using countNotZero this time) and that it needs to be decremented. (if not for the last iteration)

Now coming to the decrement_work function, I use a while-loop running for two times here as well, which will do two sets of decrements (-10 each) to count. Under it, I first set the mutex lock and wait till the incrementing thread sends a signal for the countNotZero condition variable (i.e. after it updates the value of count to 10). Once it receives the signal, it proceeds to decrement count back to 0 under a for loop. Following that, it sends a signal back to the incrementing thread that count is zero, before unlocking the mutex and exiting the loop if it is done with the second round of decrements. 

This simulates a ping-pong signalling trend between the incrementing thread and the decrementing thread via the use of condition variables, and as a result, the shared counter gets incremented, then decremented, then incremented again, then decremented again, then incremented yet again by counts of 10, from 0 to 10, 10 to 0, 0 to 10, 10 to 0 and then finally from 0 to 10, when the incrementing thread runs the for-loop within the last iteration of the while-loop under increment_work().

The main function follows the usual norm, with the declaration, initialization (with the struct arguments) and joining of the two pthreads used in this program (named as incrementer and decrementer) plus the initialization and uninitialization (at the end) of the mutex lock and the two condition variables. 
</details>

# Compilation
```c
gcc <filename>.c -lpthread -o <executablename>
```
# Testing
```c
./<executablename> | python3 <testscriptname>.py
```
