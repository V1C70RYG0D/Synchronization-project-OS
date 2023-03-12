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

// requirements - smokers and an agent
// as per internet, the problem is:
// 3 ingredients, 3 smokers
// each smoker has 1 ingredient
// no 2 smokers have same ingredient
// one agent randonly places 2 ingredients on the table
// and then smokers pick ingredients
// Solution -
// assign ingredients as value 0, 1 and 2
// make a semaphore
// each time semaphore value becomes 0 agent puts something on table and increments value of semaphore accordingly
// each smoker waits for semaphore value to become equal to the value required by him/her

atomic_int onTable;

char *ingredients[3] = {'Tobacco', 'Paper', 'Matches'};

void *smokerProcess(void *selfIngred)
{
    int ingred = *((int *)selfIngred);
    while (1)
    {
        while (onTable != 3 - ingred)
            ;
        printf("smoking started : %d\n", ingred);
        // printf("Smoker no. %d having %s started smoking\n", ingred, ingredients[ingred]);
        sleep(5);
        printf("smoking finished : %d\n", ingred);
        onTable = 0;
        // sleep(5);
        // printf("Smoker no. %d having %s finished smoking\n", ingred, ingredients[ingred]);
    }
}

void *agentProcess()
{
    int whatToPlaceOnTable;
    while (1)
    {
        while (onTable != 0)
            ;
        printf("Table is empty, determining ingredients to place on table\n");
        sleep(5);
        whatToPlaceOnTable = rand() % 3;
        // printf("Placing %s and %s on table\n", ingredients[whatToPlaceOnTable], ingredients[(whatToPlaceOnTable + 1) % 3]);
        onTable = whatToPlaceOnTable + (whatToPlaceOnTable + 1) % 3;
    }
}

int main()
{
    int index[3];
    for (int i = 0; i < 3; i++)
        index[i] = i;

    // make 3 smoker threads and one agent thread
    pthread_t smokers[3];
    pthread_t agent;
    onTable = 0;

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&smokers[i], NULL, &smokerProcess, (void *)&index[i]);
    }
    pthread_create(&agent, NULL, &agentProcess, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(&smokers[i], NULL);
    }
    pthread_join(&agent, NULL);

    printf("\nSuccessfully finished execution.\n");

    return 0;
}
