#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
#define NUM_THREADS 2
 
int a = 0;
pthread_mutex_t mutex;
 
void *f(void *arg)
{
    // facem lock pe mutex
	pthread_mutex_lock(&mutex);
    // zona critica
	a += 2;
    // facem unlock pe mutex
	pthread_mutex_unlock(&mutex);
 
	pthread_exit(NULL);
}
 
int main(int argc, char *argv[]) {
	int i;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];
 
    // cream mutexul
	pthread_mutex_init(&mutex, NULL);
 
	for (i = 0; i < NUM_THREADS; i++) {
		arguments[i] = i;
		pthread_create(&threads[i], NULL, f, &arguments[i]);
	}
 
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], &status);
	}
 
        // distrugem mutex-ul
	pthread_mutex_destroy(&mutex);
 
	printf("a = %d\n", a);
 
	return 0;
}