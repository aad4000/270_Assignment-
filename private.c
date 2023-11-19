#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 8

// Define a global array of private counts, one for each thread
int private_counts[NUM_THREADS];

// Function that each thread will execute
void *thread_work(void *thread_id) {
    long id = (long)thread_id;
    // Perform some work
    for (int i = 0; i < 1000000; i++) {
        private_counts[id]++;
    }
    pthread_exit(NULL);
}

int main() {
    clock_t start_time, end_time;
    double execution_time;

    start_time = clock();

    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize private_counts array
    for (t = 0; t < NUM_THREADS; t++) {
        private_counts[t] = 0;
    }

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, thread_work, (void *)t);
        if (rc) {
            printf("Error: pthread_create() failed\n");
            exit(-1);
        }
    }

    // Wait for threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Sum up the private counts to get the final result
    int total_count = 0;
    for (t = 0; t < NUM_THREADS; t++) {
        total_count += private_counts[t];
    }

    printf("Total count: %d\n", total_count);

    end_time = clock();
    execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Execution time: %f seconds\n", execution_time);

    pthread_exit(NULL);
}
