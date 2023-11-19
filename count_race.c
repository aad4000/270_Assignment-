#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000 // Set a smaller size for demonstration purposes

int *array;
int count = 0; // Shared count variable
int NUM_THREADS; // Number of threads

// Thread function to count ones
void* count1sThread(void* arg) {
    long long start = *(long long *)arg;
    long long end = start + ARRAY_SIZE / NUM_THREADS;
    for (long long i = start; i < end; i++) {
        if (array[i] == 1) {
            // Unsafe update to shared variable
            count++;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    NUM_THREADS = atoi(argv[1]);
    if (NUM_THREADS <= 0) {
        fprintf(stderr, "Number of threads must be a positive integer\n");
        return 1;
    }

    array = malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        perror("Failed to allocate memory for the array");
        return 1;
    }

    // Fill the array with random numbers between 0 and 5
    srand((unsigned int)time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 6;
    }

    pthread_t threads[NUM_THREADS];
    long long starts[NUM_THREADS];
    long long segmentSize = ARRAY_SIZE / NUM_THREADS;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        starts[i] = i * segmentSize;
        if (pthread_create(&threads[i], NULL, count1sThread, &starts[i])) {
            perror("Failed to create thread");
            free(array);
            return 1;
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("Failed to join thread");
        }
    }

    // Print the total count of ones
    printf("Total count of ones: %d\n", count);

    // Free the allocated memory
    free(array);
    return 0;
}
