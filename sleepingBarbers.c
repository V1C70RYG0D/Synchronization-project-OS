#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>

// custom semaphore struct
typedef volatile struct
{
    volatile atomic_int val;
    volatile atomic_flag lock;
} semaphore;
semaphore mutex, cust, barb;

// initialise semaphore value
void init(semaphore *s,int value){
    s->val = value;
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

// Global variables

#define NUM_BARBER 3
#define NUM_CHAIR 5
#define NUM_CUSTOMER 10

int waiting = 0;
int free_chair = NUM_CHAIR;

/*
* The barber sleeps when there are no customer present
* If there are customers on the waiting chairs then he will cut their hair one at a time
*/

void *barber(void *arg){
    int id = *((int *)arg);
    while (1)
    {
        // Entry Section
        wait(&cust);
        wait(&mutex);
        
        // Critical Section
        waiting--;
        free_chair++;
        signal(&mutex);

        printf("Barber %d is cutting hair\n",id);
        sleep(1);

        // Exit Section
        signal(&barb);
        // sleep(1);
    }
    pthread_exit(NULL);
}

/*
* The customer wake the barber up if he is sleeping
* If he is awake they will wait and try to occupy the waiting chairs
* If the chairs are not avaiable they will leave and come after some time
*/

void *customer(void *arg){
    int id = *((int *)arg);
    int has_haircut = 0;
    while (has_haircut==0){
        // Entry Section
        wait(&mutex);

        if(free_chair <= 0){
            // Exit Section
            printf("Customer %d left without hair cut\n",id);
            signal(&mutex);
            sleep(2);
        }
        else{
            // Critical Section
            free_chair --;
            printf("There are %d free chairs\n",free_chair);
            has_haircut = 1;
            sleep(0.5);
            
            // Exit Section
            signal(&cust);
            signal(&mutex);
            wait(&barb);
            printf("Customer %d has got hair cut\n",id);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
    pthread_t t_barber[NUM_BARBER], t_customer[NUM_CUSTOMER];
    int cust_id[NUM_CUSTOMER],barb_id[NUM_BARBER];
    init(&barb,0);
    init(&cust,0);
    init(&mutex,1);
    // pthread_create(&t_barber,NULL,barber,NULL);
    for (size_t i = 0; i < NUM_CUSTOMER; i++)
    {
        cust_id[i] = i+1;
        pthread_create(&t_customer[i],NULL,customer,&cust_id[i]);
    }
    for (size_t i = 0; i < NUM_BARBER; i++)
    {
        barb_id[i] = i+1;
        pthread_create(&t_barber[i],NULL,barber,&barb_id[i]);   
    }
    for (size_t i = 0; i < NUM_CUSTOMER; i++)
    {
        pthread_join(t_customer[i],NULL);
    }  
    printf("\nSuccessfully finished execution.\n");
    return 0;
}