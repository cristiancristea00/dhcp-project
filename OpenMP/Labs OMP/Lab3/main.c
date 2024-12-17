#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_THREADS 4

// writing to the file
void write_file(char *filename, char *type, double time, long sum) {
    FILE *file = fopen(filename, "w+");
    fprintf(file, "Type: %s\nTime: %lf\nSum: %ld", type, time, sum);
    fclose(file);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    int size, i;
    int *numbers;

    long sum1 = 0, sum2 = 0, sum3 = 0, sum = 0;
    double t1, t2;
    double time1, time2, time3;
    omp_set_num_threads(NO_THREADS);

    // to parallelize the whole piece below

    // pay attention to the read part of the file
    fscanf(file, "%d", &size);              //first line has the number of numbers in the file
    numbers = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        fscanf(file, "%d", &numbers[i]);    //put the numbers in a vector
    }
    fclose(file);

    // to parallelize in 3 modes - atomic, critical and reduction, to measure execution times
    for (i = 0; i < size; i++) {
        sum += numbers[i];
    }

    printf("Sum in serial: %ld\n", sum);

    #pragma omp parallel
    {
        // Atomic
        #pragma omp master
        {
            t1 = omp_get_wtime();
            #pragma omp parallel for ordered
            for (i = 0; i < size; i++) {
                #pragma omp ordered
                {
                    #pragma omp atomic
                    sum1 += numbers[i];
                }
            }
            t2 = omp_get_wtime();
            time1 = t2 - t1;
        }
       

        // Critical
        #pragma omp master
        {
            t1 = omp_get_wtime();
            #pragma omp parallel for ordered
            for (i = 0; i < size; i++) {
                #pragma omp ordered
                {
                    #pragma omp critical
                    sum2 += numbers[i];
                }
            }
            t2 = omp_get_wtime();
            time2 = t2 - t1;
        }
       

        // Reduction
        #pragma omp master
        {
            t1 = omp_get_wtime();
            #pragma omp parallel for ordered reduction(+:sum) 
            for (i = 0; i < size; i++) {
                #pragma omp ordered
                sum3 += numbers[i];
            }
            t2 = omp_get_wtime();
            time3 = t2 - t1;
        }
        #pragma omp barrier


        // TODO: to write the execution times in 3 files using sections (each writing in a section)

        #pragma omp sections
        {
            #pragma omp section
            {
                write_file("atomic_times", "Time used using atomic: ", time1, sum1);
            }
        }

        #pragma omp sections
        {
            #pragma omp section
            {
                write_file("critica_times", "Time used using critical: ", time2, sum2);
            }
        }

        #pragma omp sections
        {
            #pragma omp section
            {
                write_file("reduction_times", "Time used using reduction: ", time3, sum3);
            }
        }
        
    }

    free(numbers);
    return 0;
}