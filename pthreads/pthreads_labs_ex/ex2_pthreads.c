#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 
#define NUM_THREADS 8
 
struct pair {
    int first, second;
};
 
void *f(void *arg)
{
    struct pair info = *(struct pair*) arg;
    printf("First = %d; second = %d\n", info.first, info.second);
    pthread_exit(NULL);
}
 
int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int r;
    long id;
    void *status;
    struct pair arguments[NUM_THREADS];
 
    for (id = 0; id < NUM_THREADS; id++) {
        arguments[id].first = id;
        arguments[id].second = id * 2;
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
 
    pthread_exit(NULL);
}