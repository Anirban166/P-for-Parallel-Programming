# P-for-Parallel-Programming
Contains **some** parallel programs that I wrote while experimenting with the POSIX threads api.

> Emphasizing on 'some', given that there are sequential programs here as well, including ones that are worser than their normal sequential run (equivalent with no threads), due to factors such as locking overhead. 

<details>
<summary> Sequence Generation </summary>

Problem/Question:
  
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

Python script to test this program with: [test_SequenceOrder.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_SequenceCounter.py)

My solution: [SequenceCounter.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/SequenceCounter.c)
</details>

<details>
<summary> Sequence Competition </summary>

Problem/Question:
  
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

Python script to test this program with: [test_SequenceCompetition.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_SequenceCompetition.py)

My solution: [SequenceCompetition.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/SequenceCompetition.c)
</details>

<details>
<summary> Ordering Thread Execution </summary>

Problem/Question:
  
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

Python script to test this program with: [test_OrderedThreadExecutionAggregator.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_OrderedThreadExecutionAggregator.py)

My solution: [OrderedThreadExecutionAggregator.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/OrderedThreadExecutionAggregator.c)
</details>

<details>
<summary> Ping Pong </summary>

Problem/Question:
  
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

Python script to test this program with: [test_PingPong.py](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Test%20Scripts/test_PingPong.py)

My solution: [PingPong.c](https://github.com/Anirban166/P-for-Parallel-Programming/blob/main/Programs/PingPong.c)
</details>

# Compilation
```c
gcc <filename>.c -lpthread -o <executablename>
```
# Testing
```c
./<executablename> | python3 <testscriptname>.py
```
