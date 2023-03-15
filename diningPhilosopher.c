#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>

#define NUM_OF_PHILOSOPHER 10

// custom semaphore struct
typedef volatile struct
{
    volatile atomic_int val;
    volatile atomic_flag lock;
} semaphore;

semaphore sem[NUM_OF_PHILOSOPHER];

void init(semaphore *s, int val)
{
    atomic_init(&s->val, val);
    atomic_flag_clear(&s->lock);
}

// wait
int wait(semaphore *s)
{
    while (atomic_flag_test_and_set(&s->lock))
        ;
    while (atomic_load(&s->val) <= 0)
        ;
    atomic_fetch_sub(&s->val, 1);
    atomic_flag_clear(&s->lock);
    return 0;
}

// signal
int signal(semaphore *s)
{
    return atomic_fetch_add(&s->val, 1);
}

// function for philosophers
void *philosopher_process(void *num)
{
    int number = *((int *)num);

    // entry section
    printf("philosopher %d is waiting\n", number + 1);
    wait(&sem[number]);
    wait(&sem[(number + 1) % NUM_OF_PHILOSOPHER]);

    // ##### critical section #####
    printf("philosopher %d is eating\n", number + 1);
    sleep(1);
    // ##### critical section ends #####

    // exit section
    signal(&sem[number]);
    signal(&sem[(number + 1) % NUM_OF_PHILOSOPHER]);
    printf("philosopher %d is done eating\n", number + 1);
    pthread_exit(NULL);
}

void *philosopher_process_last(void *num)
{
    int number = *((int *)num);

    // entry section
    printf("philosopher %d is waiting\n", number + 1);
    wait(&sem[(number + 1) % NUM_OF_PHILOSOPHER]);
    wait(&sem[number]);

    // ##### critical section #####
    printf("philosopher %d is eating\n", number + 1);
    sleep(1);
    // ##### critical section ends #####

    // exit section
    signal(&sem[number]);
    signal(&sem[(number + 1) % NUM_OF_PHILOSOPHER]);
    printf("philosopher %d is done eating\n", number + 1);
    pthread_exit(NULL);
}

int main()
{
    int index[NUM_OF_PHILOSOPHER];
    for (int i = 0; i < NUM_OF_PHILOSOPHER; i++)
        index[i] = i;
    for (int i = 0; i < NUM_OF_PHILOSOPHER; i++)
        init(&sem[i], 1);
    pthread_t philosophers[NUM_OF_PHILOSOPHER];
    for (int i = 0; i < NUM_OF_PHILOSOPHER - 1; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher_process, (void *)&index[i]);
    }
    pthread_create(&philosophers[NUM_OF_PHILOSOPHER - 1], NULL, philosopher_process_last, (void *)&index[NUM_OF_PHILOSOPHER - 1]);

    // waiting for threads to execute
    for (int i = 0; i < NUM_OF_PHILOSOPHER - 1; i++)
    {
        pthread_join(philosophers[i], NULL);
    }
    return 0;
}