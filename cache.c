#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> // For uintptr_t

#define NUM_THREADS 4

// Determine the size of the L1 cache (you can adjust this based on your system)
#define CACHE_LINE_SIZE 64

// Define a struct to hold the private count and padding to fill the cache line
struct CacheLine {
    int private_count;
    char padding[CACHE_LINE_SIZE - sizeof(int)];
};

// Define a global array of private counts, one for each thread
struct CacheLine private_counts[NUM_THREADS];

// Function that each thread will execute
void *thread_work(void *thread_id) {
    long id = (long)thread_id;
    // Perform some work
    for (int i = 0; i < 1000000; i++) {
        private_counts[id].private_count++;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize private_counts array
    for (t = 0; t < NUM_THREADS; t++) {
        private_counts[t].private_count = 0;
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
        total_count += private_counts[t].private_count;
    }

    printf("Total count: %d\n", total_count);

    pthread_exit(NULL);
}
