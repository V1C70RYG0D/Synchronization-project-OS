/*
@Sumit Kumar 
import threading
import time

eating = 0
waiting = 0
mutex = threading.Lock()
block = threading.Condition(mutex)

def sushi_bar(customer_id):
    global eating, waiting
    while True:
        with mutex:
            if eating == 5:
                waiting += 1
                print(f"waiting customer...{customer_id}")
                block.wait()
            else:
                eating += 1
            print(f"Eating customer...{customer_id}")
        time.sleep(3)
        with mutex:
            eating -= 1
            print(f"\tLeaving customer...{customer_id}")
            if eating == 0 and waiting > 0:
                n = min(waiting, 5)
                waiting -= n
                eating += n
                for i in range(n):
                    block.notify()
        time.sleep(3)

# create threads for customers
customers = []
for i in range(8):
    customers.append(threading.Thread(target=sushi_bar, args=(i+1,)))

# start threads
for customer in customers:
    customer.start()

# wait for threads to finish
for customer in customers:
    customer.join()

*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NO_OF_CUSTOMERS 8

pthread_t customers[NO_OF_CUSTOMERS];
int eating = 0, waiting = 0;

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} custom_semaphore;

custom_semaphore block, mutex;

void init(custom_semaphore* sem, int value) {
    sem->value = value;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void wait(custom_semaphore* sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value--;
    pthread_mutex_unlock(&sem->mutex);
}

void signal(custom_semaphore* sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

void *sushi_bar(void *arg)
{
    while (1)
    {
        int i, n;
        wait(&mutex);
        if (eating == 5)
        {
            waiting += 1;
            signal(&mutex);
            printf("\twaiting customer...%d\n", *(int *)arg);
            wait(&block);
        }
        else
        {
            eating += 1;
            signal(&mutex);
        }
        printf("Eating customer...%d\n", *(int *)arg);
        sleep(3);
        wait(&mutex);
        eating -= 1;
        printf("\t\tLeaving customer...%d\n", *(int *)arg);
        if (eating == 0 && waiting)
        {
            if (waiting < 5)
                n = waiting;
            else
                n = 5;
            waiting -= n;
            eating += n;
            for (i = 0; i < n; i++)
                signal(&block);
        }
        signal(&mutex);
        sleep(3);
    }
}

int main()
{
    int i = 0;
    int customer_id[NO_OF_CUSTOMERS];
    for (i = 0; i < NO_OF_CUSTOMERS; i++)
        customer_id[i] = i + 1;

    init(&mutex, 1);
    init(&block, 0);

    for (i = 0; i < NO_OF_CUSTOMERS; i++)
        pthread_create(&customers[i], NULL, sushi_bar, &customer_id[i]);

    while (1)
        ;

    return 0;
}
