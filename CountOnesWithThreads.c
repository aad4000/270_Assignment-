#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 1000    // 1 Billion - Large size, ensure sufficient memory!
#define NUM_THREADS  32       // Number of threads

// Structure to pass data to threads
typedef struct {
    int *array;  // Pointer to the array
    long long start;
    long long end;
    int count;
} ThreadData;

// Thread function to count ones
void* count1sThread(void* arg) {
    ThreadData *data = (ThreadData *)arg;
    data->count = 0;
    for (long long i = data->start; i < data->end; i++) {
        if (data->array[i] == 1) {
            data->count++;
        }
    }
    pthread_exit(NULL);
}

int main() {
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Failed to allocate memory for the array.\n");
        return 1;
    }

    srand(time(NULL)); // Seed the random number generator

    // Fill the array with random numbers between 0 and 5
    for(long long i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 6;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    long long segmentSize = ARRAY_SIZE / NUM_THREADS;

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].array = array;  // Pass the array pointer to each thread
        threadData[i].start = i * segmentSize;
        threadData[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * segmentSize;
        pthread_create(&threads[i], NULL, count1sThread, (void *)&threadData[i]);
    }

    // Wait for threads to finish and gather results
    int totalOnes = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        totalOnes += threadData[i].count;
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print the total count of ones and the time taken
    printf("Total number of ones in the array: %d\n", totalOnes);
    printf("Time taken: %f seconds\n", cpu_time_used);

    free(array); // Free the allocated memory

    return 0;
}
