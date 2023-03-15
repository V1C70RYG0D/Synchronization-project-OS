#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// requirements - smokers and an agent
// as per internet, the problem is:
// 3 ingredients, 3 smokers
// each smoker has 1 ingredient
// no 2 smokers have same ingredient
// one agent randonly places 2 ingredients on the table
// and then smokers pick up other required ingredients and start smoking
// Solution -
// assign ingredients as value 0, 1 and 2
// smoker waits till sum of ingredients on the table and ingredient already has is 3. 
// make a semaphore
// each time semaphore value becomes 0 agent puts something on table and increments value of semaphore accordingly
// each smoker waits for semaphore value to become equal to the value required by him/her


// custom semaphore struct
typedef volatile struct{
    volatile atomic_int val;
    volatile atomic_flag lock;
} semaphore;

semaphore mutex;

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


atomic_int onTable;

char ingredients[3][15] = {"Tobacco", "Paper", "Matches"};

void *smokerProcess(void *selfIngred) {
    int ingred = *((int *)selfIngred);
    while (1) {
        while (onTable != 3 - ingred)
            ;
        printf("-> Smoker %d with ingredient %s smoking started. \n", ingred, &ingredients[ingred]);
        sleep(3);
        printf("-> Smoker %d finished smoking.\n\n", ingred);

        wait(&mutex);
        onTable = 0;
        signal(&mutex);
    }
}

void *agentProcess() {
    int ingred1;
    int ingred2;
    while (1) {
        while (onTable != 0)
            ;
        printf("Table is empty, determining ingredients to place on table\n");
        sleep(3);
        ingred1= rand() % 3;
        ingred2 = (ingred1 + 1) % 3;
        printf("Selected ingredients: %s and %s.\n", &ingredients[ingred1],&ingredients[ingred2]);

        wait(&mutex);
        onTable = ingred1 + ingred2;
        signal(&mutex);

    }
}

int main() {
    
    //initialize mutex = 1
    signal(&mutex);
    
    int index[3];
    for (int i = 0; i < 3; i++)
        index[i] = i;

    // make 3 smoker threads and one agent thread
    pthread_t smokers[3];
    pthread_t agent;
    onTable = 0;

    for (int i = 0; i < 3; i++) {
        pthread_create(&smokers[i], NULL, &smokerProcess, (void *)&index[i]);
    }
    pthread_create(&agent, NULL, &agentProcess, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(&smokers[i], NULL);
    }
    pthread_join(&agent, NULL);

    printf("\nSuccessfully finished execution.\n");

    return 0;
}
