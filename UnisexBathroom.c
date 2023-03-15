/*
  @author: Priyansh Rathi (@techiepriyansh)

  Unisex Bathroom Synchronization Problem
  =======================================
  * There are two types of employees (processes): males and females
  * They have to use a shared resource "bathroom"
  * The synchronization constraints are as follows
    - There cannot be males and females at the same time in the "bathroom"
    - There should never be more than three employees (processes) using
      the "bathroom" at the same time

  The pseudocode for the solution is as follows:
  * initialize:
    - male_count = 0, female_count = 0
    - male_count_mutex = 1, female_count_mutex = 1
    - male_multiplex = 3, female_multiplex = 3
    - empty = 1, turnstile = 1

  * male process:
    ```
    wait(turnstile);
      wait(male_count_mutex);
        male_count++;
        if (male_count == 1) {
          wait(empty);
        }
      signal(male_count_mutex);
    signal(turnstile);

    wait(male_multiplex);
      // use bathroom
    signal(male_multiplex);

    wait(male_count_mutex);
      male_count--;
      if (male_count == 0) {
        signal(empty);
      }
    signal(male_count_mutex);
    ```

  * female process:
    -- similar to the male process --
    
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_OF_MALES   20
#define NO_OF_FEMALES 20

#define TU 1000000 // Time Unit in microseconds; currently 1 TU = 1 sec

pthread_t males[NO_OF_MALES];
pthread_t females[NO_OF_FEMALES];

/* Implement semaphore using pthread mutex */
typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} semaphore;

void init(semaphore* sem, int value) {
    sem->value = value;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void wait(semaphore* sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value--;
    pthread_mutex_unlock(&sem->mutex);
}

void signal(semaphore* sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

/* Define variables and semaphores to be used */
int male_count, female_count;
semaphore male_count_mutex, female_count_mutex,
          male_multiplex, female_multiplex,
          empty, turnstile,
          printf_mutex;

/* Male process */
void *male_process(void *args)
{
  int pn = (int)args; // male process number

  wait(&turnstile);
    wait(&male_count_mutex);
      male_count++;
      if (male_count == 1) {
        wait(&empty);
      }
    signal(&male_count_mutex);
  signal(&turnstile);

  wait(&male_multiplex);
    wait(&printf_mutex);
      printf("male process # %d entering the bathroom\n", pn);
      fflush(stdout);
    signal(&printf_mutex);
 
    // using bathroom
    usleep(TU);

    wait(&printf_mutex);
      printf("male process # %d exiting the bathroom\n", pn);
      fflush(stdout);
    signal(&printf_mutex);
  signal(&male_multiplex);

  wait(&male_count_mutex);
    male_count--;
    if (male_count == 0) {
      signal(&empty);
    }
  signal(&male_count_mutex);
}

/* Female process */
void *female_process(void *args)
{
  int pn = (int)args; // female process number

  wait(&turnstile);
    wait(&female_count_mutex);
      female_count++;
      if (female_count == 1) {
        wait(&empty);
      }
    signal(&female_count_mutex);
  signal(&turnstile);

  wait(&female_multiplex);
    wait(&printf_mutex);
      printf("female process # %d entering the bathroom\n", pn);
      fflush(stdout);
    signal(&printf_mutex);

    // using bathroom
    usleep(2*TU);

    wait(&printf_mutex);
      printf("female process # %d exiting the bathroom\n", pn);
      fflush(stdout);
    signal(&printf_mutex);
  signal(&female_multiplex);

  wait(&female_count_mutex);
    female_count--;
    if (female_count == 0) {
      signal(&empty);
    }
  signal(&female_count_mutex);
}

/* Driver code */
int main()
{
    init(&male_count_mutex, 1);
    init(&female_count_mutex, 1);

    init(&male_multiplex, 3);
    init(&female_multiplex, 3);

    init(&empty, 1);
    init(&turnstile, 1);

    init(&printf_mutex, 1);


    // randomly spawn male and female processes
    int i = 0;
    int j = 0;
    
    srand(time(NULL));

    while (i < NO_OF_MALES || j < NO_OF_FEMALES) {
      int turn = rand() % 2;

      if (turn == 0 && i < NO_OF_MALES) {
        pthread_create(&males[i], NULL, male_process, (void *)(i+1));
        i++;
      }

      if (turn == 1 && j < NO_OF_FEMALES) {
        pthread_create(&females[j], NULL, female_process, (void *)(j+1));
        j++;
      }
    }


    // wait for the threads to run through completion
    for (int i = 0; i < NO_OF_MALES; i++) {
      pthread_join(males[i], NULL);
    }
    
    for (int i = 0; i < NO_OF_FEMALES; i++) {
      pthread_join(females[i], NULL);
    }

    return 0;
}
