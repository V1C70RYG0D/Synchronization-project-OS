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

## Demo Video Link
https://drive.google.com/file/d/1fC1JbBpo2hTfkM7_uceoX8TMJwQYyUdR/view?usp=share_link

## Problem Description with Solutions
---
### Producer Consumer Problem
##### Description:
The producer-consumer problem is defined as follows:

There is a buffer, Producer process, and a Consumer process. The Producer process creates an item and adds it to the shared buffer. The Consumer process takes items out of the shared buffer and “consumes” them.

Certain conditions must be met by the Producer and the Consumer processes to have consistent data synchronization:

1. The Producer process must not produce an item if the shared buffer is full.

2. The Consumer process must not consume an item if the shared buffer is empty.

3. Access to the shared buffer must be mutually exclusive; this means that at any given instance, only one process should be able to access the shared buffer and make changes to it.

Bounded Buffer - The size of the buffer is limited
Cyclic Buffer - The buffer is size limited and producing and consuming happens in a cyclic manner; that is the produce and consume indices are always incremented and wrapped around
Infinite Buffer - There is no limitation on size and the producer can keep producing into the buffer
##### Solution:
The solution to the Producer-Consumer problem having bounded and cyclic buffers involve three semaphore variables.

full: Tracks the space filled by the Producer process. It is initialized with a value of 0 as the buffer will have no filled spaces at the beginning
empty: Tracks the empty space in the buffer. It is initially set to BUFFER_SIZE as the whole buffer is empty at the beginning.
pc_mutex: Used for mutual exclusion so that only one process can access the shared buffer at a time. Initially set to 1

Pseudocode for Producer:
```c
wait(empty)
wait(pc_mutex)

// produce

signal(pc_mutex)
signal(full)
```
Pseudocode for Consumer:
```c
wait(full)
wait(pc_mutex)

// consume

signal(pc_mutex)
signal(empty)
```
The solution to the Producer-Consumer problem having infinite buffer involves two semaphore variables, and the buffer is implemented in a linked list fashion. The semaphores are:

**non_empty**: Tracks if the buffer is not empty so that consumer can consume from the buffer. Initially set to 0
**pc_mutex**: Used for mutual exclusion so that only one process can access the shared buffer at a time. Initially set to 1

Pseudocode for Producer:
```c
wait(pc_mutex)

// produce

signal(pc_mutex)
signal(non_empty)
```
Pseudocode for Consumer:
```c
wait(non_empty)
wait(pc_mutex)

// consume

signal(pc_mutex)
```
### Dining Philosophers' Problem
##### Description:
The Dining Philosopher problem is a classcial synchronisation problem in which there are n philosophers on a round table with a chopstick between adjacent philosophers. A philosopher may eat if he can pick up the two chopsticks adjacent to him.
##### Solution:
Every philosopher picks up the chopsticks one by one and eats and then puts them down when he is done eating. To make our solution deadlock-free, we have defined a separate function for the last philospher process, which picks up chopsticks in the opposite order, thus preventing deadlock.
### Readers Writers Problem
##### Description:
The Reader-Writer Problem is a classic scenario in Computer Science where multiple processes concurrently access shared resources.The critical section can be accessed by only one writer or by multiple readers simultaneously at any point of time. Semaphores are used to prevent conflicts and ensure proper process synchronization.
##### Solution:
Here, we have presented a Starve-free Solution with pthread implementation using custom semaphores. We used 3 semaphores to make the problem starve free, used pthreads to make the processes parallel and managed the whole synchronization processes using our custom semaphores. The implementation is done keeping in mind that multiple Readers can access the Critical Section together but when a writer is accessing the Critical Section no other Reader or Writer should access it.
### Cigarette Smokers' Problem
##### Description:
There are 3 ingredients involved and 3 smokers involved with each having infinite supply of one along with an agent. All the three ingredients are required simultaneously by a smoker to smoke. Whenever the agent sees the table empty, he arbitrarily puts 2 ingredients on the table. 
##### Solution:
We maintain an atomic integer which tells the state of the table(what ingredients are there in case table is not empty or is the table empty) and then make each smoker wait till the two ingredients placed by agent are such that non of them are with the smoker which picks up the ingredients. The ingredients to be put on table are decided randomly.
### Sleeping Barbers' Problem
##### Description:
In computer science, the sleeping barber problem is a classic inter-process communication and synchronization problem that illustrates the complexities that arise when there are multiple operating system processes. The problem conatins multiple barbers with multiple barber chairs, a waiting room with more chairs for waiting customers. The following rules apply: 
* If there are no customers, the barbers fall asleep in the chair
* A customer must wake the barber if he is asleep
* If a customer arrives while all the barbers are working, the customer leaves if all chairs are occupied and sits in an empty chair if it's available
* When a barber finishes a haircut, he inspects the waiting room to see if there are any waiting customers and falls asleep if there are none
##### Solution:
The pseudocode is as follows:
* Initialization
```
int has_haircut = 0
int waiting = 0
int free_chair = 10
semaphore mutex = 1, cust = 0, barb = 0
```
* Barbers' pseudocode
```c
while (1)
    {
        // Entry Section
        wait(&cust);
        wait(&mutex);
        
        // Critical Section
        waiting--;
        free_chair++;
        signal(&mutex);

        // Barber is Cutting hair

        // Exit Section
        signal(&barb);
        sleep(1);
    }
```

* Customers' pseudocode
```c
while (has_haircut==0){
        // Entry Section
        wait(&mutex);

        if(free_chair <= 0){
            // Exit Section
            //Customer left without haircut
            signal(&mutex);
            sleep(1);
        }
        else{
            // Critical Section
            free_chair --;
            // No. of free chairs available = free_chair
            has_haircut = 1;
            
            // Exit Section
            signal(&cust);
            signal(&mutex);
            wait(&barb);
            // Customer is getting Haircut
        }
    }
```

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
