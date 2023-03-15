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

The Sushi Bar problem is a classic synchronization problem that involves a sushi bar with a limited number of seats
where customers can eat sushi. The problem assumes that there is one sushi chef who prepares sushi and places them
on a bar, and multiple customers who visit the bar to eat sushi.
The problem is to implement a solution where the chef and the customers can access the sushi bar in a synchronized
manner so that the following rules are followed:

* The chef can only place sushi on the bar if there is an empty seat.
* The customer can only sit and eat sushi if there is sushi on the bar.
* There are limited seats at the sushi bar, and if all seats are occupied, the customers who arrive later must wait until a seat becomes available.

##### Solution:

* The code defines two custom semaphores named "block" and "mutex" to synchronize access to the sushi bar.

* The "init" function initializes the semaphores and their associated mutex and condition variables.

* The "wait" function waits for a semaphore to become available, and if the semaphore is not available,
  it waits using the associated condition variable.

* The "signal" function signals a waiting thread that the semaphore is available.

* The "sushi_bar" function is the main function that implements the solution to the Sushi Bar problem.
  The function runs in an infinite loop and represents a single customer.
  It first waits for the "mutex" semaphore to become available and then checks if there are any empty
  seats at the sushi bar. If there are no empty seats, the customer increments the "waiting" variable
  and waits for the "block" semaphore to become available. If there are empty seats,
  the customer increments the "eating" variable and signals the "mutex" semaphore.
  The customer then eats sushi for 3 seconds, decrements the "eating" variable,
  and checks if there are any waiting customers. If there are waiting customers and empty seats,
  the customer signals the "block" semaphore to allow a waiting customer to eat sushi.

* The "main" function initializes the semaphores and creates threads for each customer.
  The function then runs in an infinite loop to keep the program running.

###### Pseudo-Code!
```py
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
```
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
