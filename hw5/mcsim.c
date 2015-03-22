// --------------------------------
// Do not modify this code!
// --------------------------------

#include "mcsim.h"

// compute the difference of times in nanoseconds (use doubles for big #s)
double mydifftime(struct timespec *tp0, struct timespec *tp1) {

  double secs = (int) (tp1->tv_sec - tp0->tv_sec);
  double nanosecs = (int) (tp1->tv_nsec - tp0->tv_nsec);

  if (nanosecs < 0) {
    nanosecs += 1000000000;
    secs--;
  }

  return secs*1000000000+nanosecs;

} // end mydifftime function

// thread computation using monte carlo to compute Pi
void* th_routine(void* th_args) {

  long tid = (long) th_args;   // NOTE:  we passed in tid value in create
  struct drand48_data buf;     // state information for re-entrant drand48
  double x, y;       // randomly generated x and y in [0, 1)
  long i;            // loop control variable
  long count = 0;    // local count

  //printf("starting thread %ld...\n", tid);
  
  srand48_r(tid, &buf);   // the _r version are thread safe (read man page)
  
  for (i = 0; i < numits; i++) {  // numits is global variable
    drand48_r(&buf, &x);  // uniform random x value in [0, 1)
    drand48_r(&buf, &y);  // uniform random y value in [0, 1)
    
    if (x*x + y*y <= 1)   // no need to do square root (think why?)
      count++;            // count how many points within quarter circle
  }

  gcount += count;   // add local count to global count -- what can go wrong?

  return 0;

} // end th_routine function
