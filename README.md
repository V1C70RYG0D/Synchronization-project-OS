# Synchronization-project

## List of Problems:
---
* Producer Consumer Problem(Bounded Buffer, Cyclic Buffer, Infinite Buffer)
* Dining Philosophers' Problem
* Readers Writers Problem
* Cigarette Smokers' Problem
* Sleeping Barbers' Problem
* Sushi Bar Problem
* Unisex Bathroom Problem

## Problem Description with Solutions
---
### Producer Consumer Problem
##### Description:
##### Solution:
### Dining Philosophers' Problem
##### Description:
##### Solution:
### Readers Writers Problem
##### Description:
The Reader-Writer Problem is a classic scenario in Computer Science where multiple processes concurrently access shared resources.The critical section can be accessed by only one writer or by multiple readers simultaneously at any point of time. Semaphores are used to prevent conflicts and ensure proper process synchronization.
##### Solution:
Here, we have presented a Starve-free Solution with pthread implementation using custom semaphores. We used 3 semaphores to make the problem starve free, used pthreads to make the processes parallel and managed the whole synchronization processes using our custom semaphores. The implementation is done keeping in mind that multiple Readers can access the Critical Section together but when a writer is accessing the Critical Section no other Reader or Writer should access it.
### Cigarette Smokers' Problem
##### Description:
##### Solution:
### Sleeping Barbers' Problem
##### Description:
##### Solution:
### Sushi Bar Problem
##### Description:
##### Solution:
### Unisex Bathroom Problem
##### Description:
* There are two types of employees (processes): males and females
* They have to use a shared resource "bathroom"
* The synchronization constraints are as follows
  - There cannot be males and females at the same time in the "bathroom"
  - There should never be more than three employees (processes) using
      the "bathroom" at the same time
##### Solution:
  The pseudocode for the solution is as follows:
  * Initialize:
    - `male_count = 0, female_count = 0`
    - `male_count_mutex = 1, female_count_mutex = 1`
    - `male_multiplex = 3, female_multiplex = 3`
    - `empty = 1, turnstile = 1`

  * Male process:
    ```c
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

  * Female process:  
    -- similar to the male process --

---
## Team members:
* **Priyansh Rathi** 20116073 B.Tech ECE 3Y
* **Ahsen Kamal** 20116073 B.Tech ECE 3Y
* **Sudarshan Shenoy** 20116073 B.Tech ECE 3Y
* **Patel Jainil Subhaskumar** 21114072 B.Tech CSE 2Y
* **Gyanendra Banjare** 21114040 B.Tech CSE 2Y
* **Manas Ghandat** 21114037 B.Tech CSE 2Y
* **Subhajit Biswas** 21114100 B.Tech CSE 2Y
* **Sumit Kumar** 21114101 B.Tech CSE 2Y
* **Manan Garg** 21114056 B.Tech CSE 2Y
* **Aarya Mukul Agrawal** 21114001 B.Tech CSE 2Y
