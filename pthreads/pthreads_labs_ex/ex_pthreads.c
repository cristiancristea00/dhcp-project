#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 
#define NUM_THREADS 2
 
void *f(void *arg)
{
    long id = *(long*) arg;
    printf("Hello World din thread-ul %ld!\n", id);
    return NULL;
}
 
int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int r;
    long id;
    void *status;
    long arguments[NUM_THREADS];
 
    for (id = 0; id < NUM_THREADS; id++) {
        arguments[id] = id;
        r = pthread_create(&threads[id], NULL, f, (void *) &arguments[id]);
 
        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }
 
    for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_join(threads[id], &status);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }
 
    return 0;
}