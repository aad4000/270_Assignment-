#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define ARRAY_SIZE 1000000  // Adjust as needed for your test
int *array;
int count = 0;
int NUM_THREADS;  // This will be set based on the command line argument
pthread_mutex_t count_mutex;

// Thread function to count ones
void* count1sThread(void* arg) {
    long thread_part = (long)arg;
    long start = thread_part * (ARRAY_SIZE / NUM_THREADS);
    long end = (thread_part + 1) * (ARRAY_SIZE / NUM_THREADS);

    for (long i = start; i < end; i++) {
        if (array[i] == 1) {
            pthread_mutex_lock(&count_mutex);
            count++;
            pthread_mutex_unlock(&count_mutex);
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

    // Initialize the mutex
    pthread_mutex_init(&count_mutex, NULL);

    pthread_t threads[NUM_THREADS];
    struct timeval start_time, end_time;

    // Get the start time
    gettimeofday(&start_time, NULL);

    // Create threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, count1sThread, (void*)i);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Get the end time
    gettimeofday(&end_time, NULL);

    // Calculate the execution time
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long micros = ((seconds * 1000000) + end_time.tv_usec) - (start_time.tv_usec);

    // Print the total count of ones and the execution time
    printf("Total count of ones: %d\n", count);
    printf("Execution time is %ld seconds and %ld microseconds\n", seconds, micros);

    // Clean up
    pthread_mutex_destroy(&count_mutex);
    free(array);
    
    return 0;
}
