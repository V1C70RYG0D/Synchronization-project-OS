#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>

//Semaphore struct
typedef volatile struct
{
    volatile atomic_int val;
    volatile atomic_flag lock;
} semaphore;

semaphore CSmutex; // Semaphore to control access to Critical Section
semaphore Rmutex; // Semaphore to increment or decrement Reader Count by readers
semaphore EntryMutex; // Helps make the Reader Writer problem STARVE FREE by assigning equal priority to both
semaphore waiting;

int eating = 0, waiting_count = 0;

// wait
int wait(semaphore *s)
{
    while (atomic_flag_test_and_set(&s->lock));

    while (atomic_load(&s->val) <= 0);

    atomic_fetch_sub(&s->val, 1);
    atomic_flag_clear(&s->lock);

    return 0;
}

// signal
int signal(semaphore *s)
{
    return atomic_fetch_add(&s->val, 1);
}

// Sushi bar function
void *sushi_bar(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        printf("Customer %d trying to enter\n", id);
        wait(&EntryMutex);
        printf("Customer %d entered\n", id);
        wait(&Rmutex);
        if (eating == 5)
        {
            signal(&Rmutex);
            printf("Customer %d is waiting...\n", id);
            wait(&waiting);
            wait(&Rmutex);
        }
        signal(&EntryMutex);
        printf("Customer %d starting to eat\n", id);
        eating += 1;
        signal(&Rmutex);
        sleep(3);
        printf("Customer %d finished eating\n", id);
        wait(&Rmutex);
        eating -= 1;
        if (waiting_count > 0)
        {
            signal(&waiting);
            waiting_count -= 1;
        }
        else
        {
            signal(&Rmutex);
        }
    }
}

int main()
{
    int i;
    pthread_t customers[8];
    int customer_id[8];

    signal(&CSmutex);   //
    signal(&Rmutex);    //  INITIALIZING ALL SEMAPHORES
    signal(&EntryMutex);//
    signal(&waiting);   //

    for (i = 0; i < 8; i++)
        customer_id[i] = i + 1;

    for (i = 0; i < 8; i++)
        pthread_create(&customers[i], NULL, sushi_bar, &customer_id[i]);

    for (i = 0; i < 8; i++)
        pthread_join(customers[i], NULL);

    return 0;
}
