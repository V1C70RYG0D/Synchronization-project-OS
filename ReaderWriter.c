//Readers-Writers problem using Custom Semaphores and p-thread implementation (Starve-free)
//Here we have shown the Synchronization process using only 5 Readers and 5 Writers

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
    printf("Reader %d increments count to %d \n",number+1,Rcount);
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
    usleep(3);

    wait(&Rmutex);
    Rcount--;
    printf("Reader %d decrements count to %d \n",number+1,Rcount);
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
    printf("Writer %d enters and closes entry \n",number+1);
    wait(&CSmutex);
    printf("Writer %d in Critical Section\n",number+1);
    signal(&EntryMutex);
    printf("Writer %d opens entry now\n",number+1);
      //                //
     //Critical Section//
    //                //
    printf("Writer %d does work in Critical section\n",number+1);

    signal(&CSmutex);
    printf("Writer %d out of Critical Section\n",number+1);
}

int main()
{
    pthread_t r[5], w[5];
    int id[5]; // Reader-Writer process id to distinguish them
    Rcount = 0;

    signal(&CSmutex);   //
    signal(&Rmutex);    //  INITIALIZING ALL SEMAPHORES
    signal(&EntryMutex);//

    for (int i = 0; i < 5; i++) {
        id[i] = i; // assigning id

        pthread_create(&r[i], NULL, reader, (void *)&id[i]); // Creating pthreads for Reader processes
        pthread_create(&w[i], NULL, writer, (void *)&id[i]); // Creating pthreads for Writer processes

    }
    // waiting for threads to execute
    for (int i = 0; i < 5; i++)
        pthread_join(r[i], NULL); 

    for (int i = 0; i < 5; i++)
        pthread_join(w[i], NULL);
}

// Terminal Commands to run the Given code:
// gcc ReaderWriter.c -o rw -lpthread
// ./rw