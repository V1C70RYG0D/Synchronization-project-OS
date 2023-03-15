//Readers-Writers problem using Custom Semaphores and p-thread implementation (Starve-free)
//Here we have shown the Synchronization process using only 10 Readers and 10 Writers

/*
DESCRIPTION
~~~~~~~~~~~~

We have Implemented our custom Semaphores using volatile struct named 'semaphore'.
3 semaphores have been declared CSmutex,Rmutex and EntryMutex

CSmutex for Critical Section entry
Rmutex for mutual exclusion while updation of ReaderCount
EntryMutex for equal priority of Readers and Writers to access the Critical Section(Responsible for Starve-free)

wait and signal functions have been implemented for the custom Semaphores

Reader function implemented in a way that multiple readers can access the Critical Section together

Writer function implemented in a way that only one writer is present in Critical section at a time

We have created pthreads for 10 readers and writers each and assigned them with their respective IDs, allowing parallelism.

Finally we joined the pthreads for the threads to wait for each other to join their results.

*/

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

int Rcount; // Readers count

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

// Reader function
void *reader(void* num){

    int number = *((int *)num); // for process id

    printf("Reader %d trying to enter\n",number+1);
    wait(&EntryMutex);
    printf("Reader %d entering\n",number+1);
    wait(&Rmutex);
    Rcount++;
    printf("Reader %d increments Reader count to %d \n",number+1,Rcount);
    if(Rcount == 1){
        wait(&CSmutex);
        printf("Critical Section openend for Readers!!!\n");
    }
    signal(&Rmutex);
    printf("Done Increment by Reader %d \n",number+1);
    signal(&EntryMutex);
    printf("Opened gates for other readers by Reader %d \n",number+1);
      //                //
     //Critical Section//
    //                //
    printf("In Critical Section Reader %d\n",number+1);
    sleep(3);

    wait(&Rmutex);
    Rcount--;
    printf("Reader %d decrements Reader count to %d \n",number+1,Rcount);
    signal(&Rmutex);
    if(Rcount == 0){
        signal(&CSmutex);
        printf("Critical Section closed for Readers!!!\n");
    }
}

//Writer function
void *writer(void* num){

    int number = *((int *)num); // for process id
    
    printf("Writer %d trying to enter\n",number+1);
    wait(&EntryMutex);
    printf("Writer %d enters from Entry and closes entry \n",number+1);
    wait(&CSmutex);
    printf("Writer %d locks Critical Section\n",number+1);
    signal(&EntryMutex);
    printf("Writer %d opens entry now for other Readers and Writers\n",number+1);
    sleep(3);
      //                //
     //Critical Section//
    //                //
    printf("Writer %d does work in Critical section\n",number+1);

    signal(&CSmutex);
    printf("Writer %d out of Critical Section\n",number+1);
}

int main()
{
    pthread_t r[10], w[10];
    int id[10]; // Reader-Writer process id to distinguish them
    Rcount = 0;

    signal(&CSmutex);   //
    signal(&Rmutex);    //  INITIALIZING ALL SEMAPHORES
    signal(&EntryMutex);//

    for (int i = 0; i < 10; i++) {
        id[i] = i; // assigning id

        pthread_create(&r[i], NULL, reader, (void *)&id[i]); // Creating pthreads for Reader processes
        pthread_create(&w[i], NULL, writer, (void *)&id[i]); // Creating pthreads for Writer processes

    }
    // waiting for threads to execute
    for (int i = 0; i < 10; i++)
        pthread_join(r[i], NULL); 

    for (int i = 0; i < 10; i++)
        pthread_join(w[i], NULL);
}

// Terminal Commands to run the Given code:
// gcc ReaderWriter.c -o rw -lpthread
// ./rw
