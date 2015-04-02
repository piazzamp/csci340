#include <stdio.h>
#include <stdlib.h>        
#include <sys/time.h>      
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h> 

#define DEBUG 0
#define TRUE 1




/**************************************************
 
 Main Thread Function:
 void* th_main( void* th_main_args )
 
 Note: NO mutex locks or unlocks should be performed by main 
 thread, however the chopsticks array is being read by the 
 main_thread, and is vulnerable to race conditions. You must 
 consider the effect of these race conditions, and the 
 correctness of the check/comparsions and output of the 
 main_thread. 
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 1. Initialize all element values in the chopsticks array to -1
 2. Create a thread for each philosopher (hint: use philosophers[5] array)
 - If a error condition occurs, then immediately exit this function with status value 1.
 3. Execute a infinite loop that does the following:
 If a deadlock condition is found then 
 display deadlock
 terminate the loop and goto step 4
 otherwise 
 display which philosophers are eating
 and then call the delay function (you specify nanosec sleep value)
 
 An portion of example output:
 Philopsher(s) 0, 2 are eating
 Philospher(s) 1 are eating
 .
 .
 .
 Deadlock condition (0,1,2,3,4) ... terminating
 
 4. Kill each philosopher thread
 5. Exit the main thread with status value equal to 0.
 
 */

void* th_main( void* th_main_args );

/**************************************************
 
 Philosopher Thread Function: 
 void* th_phil( void* th_phil_args );
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 1. Get the philosopher id (hint: use th_phil_args)
 2. Execute an infinite loop that does the following:
 - call the delay function for thinking (you specify nanosec sleep value)
 - call the eat function (argument is the philosopher id)
 
 */

void* th_phil( void* th_phil_args );

/**************************************************
 
 Delay Function: 
 void delay( long nanosec )
 
 General purpose function that executes a nanosecond sleep.
 This function is provided to you (i.e. do not modify).
 The delay function is called by the th_main, th_phil, and eat functions.
 
 */

void delay( long nanosec );

/**************************************************
 
 Eat Function: 
 void eat( int phil_id )
 
 ------------------------------------------------
 In this function perform the following steps:
 ------------------------------------------------
 This philosopher, phil_id, picks up chopstick phil_id
 (i.e. the right chopstick) then delays for no more than 
 20,000 nanoseconds. Next the philosopher picks up the 
 left chopstick. Note: mutual exclusion is necessary when 
 updating a shared resource. After having picked up both 
 chopsticks (as described) the philosopher will delay a 
 number of nanoseconds that is determined by you experimentally.
 After the delay completes, the philosopher will release 
 the two chopsticks in the reverse order in which they were
 picked up.
 
 */

void eat( int phil_id );
