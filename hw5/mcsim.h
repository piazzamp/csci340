#include <stdlib.h>        // for drand48_r(), strtod(), malloc()
#include <sys/time.h>      // for clock_gettime()
#include <sys/resource.h>  // for getrlimit()

// "extern" keyword says that these variables are defined somewhere (hw5.c)
extern double gcount;    // make a double to handle large sums (billions+)
extern long numits;

// compute the difference of times in nanoseconds (again, use doubles)
double mydifftime(struct timespec *tp0, struct timespec *tp1);

// thread computation using monte carlo to compute Pi
void* th_routine(void* th_args);
