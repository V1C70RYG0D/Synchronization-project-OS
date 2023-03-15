#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#define BUFFER_SIZE 8
#define MY_RAND_MAX 99
#define PRINT_BUFFER true

void *producer_function(void *ptr);
void *consumer_function(void *ptr);

void printBuffer(int buffer[BUFFER_SIZE]){
     for (int i = 0; i < BUFFER_SIZE; ++i)
     {
          printf("%d ", buffer[i]);
     }
     printf("\n");
}

int getRandomNumber(int MAX){
     int n;
     n = (rand() % MAX) + 1;
     return n;
}

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} custom_semaphore;

void custom_init(custom_semaphore* sem, int value) {
    sem->value = value;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void custom_wait(custom_semaphore* sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value--;
    pthread_mutex_unlock(&sem->mutex);
}

void custom_signal(custom_semaphore* sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

int get_value(custom_semaphore* sem) {
    int value;
    pthread_mutex_lock(&sem->mutex);
    value = sem->value;
    pthread_mutex_unlock(&sem->mutex);
    return value;
}

custom_semaphore full, empty, pc_mutex;
int buffer[BUFFER_SIZE];
int buffer_in = 0;
int buffer_out = 0;
bool active = true;

int main(int argc, char**argv) {

    srand(time(NULL));

    int sleepTime;
    int numberOfProducers;
    int numberOfConsumers;

    switch (argc){
        case 4:
            sleepTime = atoi(argv[1]);
            numberOfProducers = atoi(argv[2]);
            numberOfConsumers = atoi(argv[3]);
            printf("Timeout: %d\tNum of producers: %d\tNum of consumers: %d\n", sleepTime, numberOfProducers, numberOfConsumers);
            break;

        default:
            printf("Please enter - sleep time, producer #, consumer #\n");
            exit (0);
            break;
    }

    custom_init(&empty, BUFFER_SIZE);
    custom_init(&full, 0);
    custom_init(&pc_mutex, 1);

    pthread_t producer_threads[numberOfProducers];
    pthread_t consumer_threads[numberOfConsumers];

    int producer_id[numberOfProducers];
    int consumer_id[numberOfConsumers];

    for (int i = 1; i <= numberOfProducers; i++) {
        producer_id[i-1] = i;    
    }

    for (int i = 1; i <= numberOfConsumers; i++) {
        consumer_id[i-1] = i;
    }

    for (int i = 0; i < numberOfProducers; i++) {
        pthread_create(&producer_threads[i], NULL, producer_function, (void*) &producer_id[i]);
        printf("Creating producer thread: %d\n", i+1);
    }

    for (int i = 0; i < numberOfConsumers; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer_function, (void*) &consumer_id[i]);
        printf("Creating consumer thread: %d\n", i+1);
    }

    sleep(sleepTime);
    printf("Ending... \n");
    active = false;

    int count = 0;
    while (count < numberOfProducers) {
        pthread_join(producer_threads[count], NULL);
        count++;
    }

    count = 0;
    
    while (count < numberOfConsumers) {
        pthread_join(consumer_threads[count], NULL);
        count++;
    }

    exit(0);
}

void *producer_function(void *ptr) {

    int threadNumber = *((int *)ptr);

    do {
        int nextProduced = getRandomNumber(MY_RAND_MAX);
        
        custom_wait(&empty);
        custom_wait(&pc_mutex);

        buffer[buffer_in] = nextProduced;
        buffer_in++;
        buffer_in %= BUFFER_SIZE;

        printf("producer %d produced %d\n", threadNumber , nextProduced);
    
        if (PRINT_BUFFER) {
            printf("Buffer: ");
            printBuffer(buffer);
        }

        custom_signal(&pc_mutex);
        custom_signal(&full);

        sleep(getRandomNumber(5));
    } while (active);

    custom_signal(&full);
}

void *consumer_function(void *ptr) {

    int threadNumber = *((int *)ptr);

    do {
        int nextConsumed;

        custom_wait(&full);
        custom_wait(&pc_mutex);

        nextConsumed = buffer[buffer_out];
        buffer[buffer_out] = 0;
        buffer_out++;
        buffer_out %= BUFFER_SIZE;

        printf("consumer %d consumed %d\n", threadNumber, nextConsumed);
        
        if (PRINT_BUFFER) {
            printf("Buffer: ");
            printBuffer(buffer);
        }

        custom_signal(&pc_mutex);
        custom_signal(&empty);

        sleep(getRandomNumber(5));
    } while (active);

    custom_signal(&empty);
}