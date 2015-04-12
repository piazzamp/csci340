#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

#define DEBUG 0

/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  number of times to repeat each experiment (ie. number of runs).  The
  third parameter is the duration of the each simulation run. The
  forth parameter is a random number seed. Here is an example command
  line:

  ./hw7 10000 100 2000 1234

  This means that your program should initialize physical memory to
  10,000 units, perform 100 runs with each run taking 2000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1234.
*/

int main(int argc, char** argv){
	int memsize, runs, iterations, seed, method, i, j, sizerange, durationrange, dur, siz, total_frags = 0, total_misses = 0, total_probes = 0, result;
	// double durationscale, sizescale;
	enum mem_strategies strategy;
	char *strat_string;
	if (argc != 5){
		fprintf(stderr, "incorrect number of args. I want 4, you gave me %d\nusage: ./%s <memsize> <number of runs> <duration> <seed>\n", argc, argv[0]);
	exit(1);
	}
	memsize = atoi(argv[1]);
	runs = atoi(argv[2]);
	iterations = atoi(argv[3]);
	seed = atoi(argv[4]);
	mem_init(memsize);

	srand(seed);
	// then mult the percentages by the appropriate scale:
	durationrange = MAX_DURATION - MIN_DURATION;
	sizerange = MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;
	for (method = 0; method <= 2; method++){
		total_frags = 0;
		total_misses = 0;
		total_probes = 0;
		mem_clear();
		switch (method){
			case 0:
				strategy = FIRST;
				strat_string = "FIRST\0";
				break;
			case 1:
				strategy = NEXT;
				strat_string = "NEXT\0";
				break;
			case 2:
				strategy = BEST;
				strat_string = "BEST\0";
				break;
			default:
				fprintf(stderr, "oh golly, I broke the enum iterator\n");
				exit(1);
		}
		for (i = 0; i < runs; i++){
			// each iteration in this loop is a full test with a single
			// placement strategy

			for (j = 0; j < iterations; j++){
				// each iteration in this loop represents one time unit

				// use one set of memory for all of *iterations*
				// then clear it out and do it again (outer loop)

				// get random number in the right range
				// by getting a num between 0-range, then adding min
				dur = (rand() % (durationrange + 1)) + MIN_DURATION;
   			siz = (rand() % (sizerange + 1)) + MIN_REQUEST_SIZE;
				//if (DEBUG) { printf("using duration %d and size %d\n", dur, siz); }	
				result = mem_allocate(strategy, siz, dur);
				if (result == -1){
					total_misses += 1;
				}
				else {	
					total_probes += result;
				}
				mem_single_time_unit_transpired();
			}
			total_frags += mem_fragment_count(3);
		}
	printf("%s:\tavg fragmentation: %.3f\tavg failures: %.3f\tavg probes: %.5f\n", strat_string, ((double) total_frags) / ((double) runs), ((double) total_misses)/((double) runs), ((double) total_probes)/((double) runs));

	}
	mem_free();
  return 0;
}
