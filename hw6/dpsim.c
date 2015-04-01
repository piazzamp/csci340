#include "dpsim.h"


static const unsigned int NUM_PHILOSOPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosophers[5];

int isdead(){
	// return 1 if there is deadlock
	// assumes everyone grabs the lower-nembered chopstick first
	// (the one to the phil's right)
	// excpet p4, who grabs c4 then c0

	// cannot have critical sections within th_main, 
	// must hope that no writes happen in the following loop
	int i;
	for (i = 0; i < NUM_CHOPSTICKS; i++){
		if (chopsticks[i] == -1) return 0;
		// can't have deadlock if there's a free stick
		else if (chopsticks[i] == chopsticks[(i +1) % 5]) return 0;
		// can't have deadlock if anyone is successfully eating
	}
	return 1;
	
}

void* th_main( void* th_main_args ) {
	int i, prev = -1;	
	for (i = 0; i < NUM_CHOPSTICKS; i++){
		chopsticks[i] = -1;
	}
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		if (pthread_create(&(philosophers[i]), NULL, th_phil, &i) != 0){
			fprintf(stderr, "failed to create philosopher thread %d, terminating\n", i);
			exit(1);
		}
	}
	while (1){
		delay(8);
		if (isdead()){
			printf("deadlocked...\n");
			break;	
		}
		printf("philosopher(s) ");
		for (i = 0; i < NUM_CHOPSTICKS; i++){
			if (chopsticks[i] >= 0 && chopsticks[i] != prev) printf("%d, ", i);
			prev = chopsticks[i];
		}
		printf("are eating\n");
		prev = -1;
		if (DEBUG){
			printf("chopsticks = {");
			for (i = 0; i < NUM_CHOPSTICKS; i++){
				printf("%d ", chopsticks[i]);
			}
			printf("}\n");
		}
	}
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		pthread_kill(philosophers[i], 9);
	}
	pthread_exit(0);

} // end th_main function


void* th_phil( void* th_phil_args ) {
	int id = *((int*)th_phil_args);
	while(1){
		delay(15); // thinking...
		eat(id);
	}

} // end th_phil function


void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {
	int left = (phil_id + 1) % 5;
	if (DEBUG) printf("right: %d  left: %d\n", phil_id, left);
	if (chopsticks[phil_id] != -1 && chopsticks[phil_id] != phil_id){
		if (DEBUG) printf("right chopstick [%d] taken, sorry\n", phil_id);
		return;
	}
		// go back to thinking to wait for the right stick to free up
	pthread_mutex_lock(&(mutex[phil_id]));
	// just set the lock to write into chopsticks array
	chopsticks[phil_id] = phil_id;
	pthread_mutex_unlock(&(mutex[phil_id]));
	// delay between picking up other chopstick
	delay(7500);
	if (chopsticks[left] != -1){
		if (DEBUG) printf("left chopstick [%d] taken, sorry\n", left);
		return;
	}
		// if you grab the right chopstick but the left is taken
		// return to thinking and try again later
	pthread_mutex_lock(&(mutex[left]));
	chopsticks[left] = phil_id;
	pthread_mutex_unlock(&(mutex[left]));
	delay(200000); // eat time
	pthread_mutex_lock(&(mutex[left]));
	chopsticks[left] = -1;
	pthread_mutex_unlock(&(mutex[left]));
	delay(7500);
	pthread_mutex_lock(&(mutex[phil_id]));
	chopsticks[phil_id] = -1;
	pthread_mutex_unlock(&(mutex[phil_id]));
} // end eat function
