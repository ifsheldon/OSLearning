# Report 4

## Q1. Describe Function of `pthread_create`

This function creates a new thread in the calling function and returns with an integer(0 for success or error codes). And then the new thread starts execution by invoking `start_routine()`, which is passed to the `pthread_create()` as a pointer argument.

## Q2. Describe Function of `pthread_join`

This function waits for a specified thread to terminate and return an integer value (0 for success or error codes). If the specified thread has already terminated, it returns immediately. Moreover, if a pointer is passed to `pthread_join` to retrieve the return value of the thread, the pointer will be set pointing to the memory location of the return value.

## Q3. Describe Function of `pthread_mutex_lock`

This function tries to lock a `pthread_mutex_t` reference by its pointer. If the mutex is already locked, the calling thread blocks until the mutex becomes available. When this function return, the mutex is locked and given to the calling thread. Also, when the function returns, an integer is returned, indicating success(0) or error(error code).

## Q4. Describe Function of `pthread_cond_wait`

According to [man page](https://linux.die.net/man/3/pthread_cond_wait)

> The `pthread_cond_timedwait()` and `pthread_cond_wait()` functions shall block on a condition variable. These functions atomically release `mutex` and cause the calling thread to block on the condition variable `cond`.
>
> Upon successful return, the mutex shall have been locked and shall be owned by the calling thread.

Simply, `pthread_cond_wait()` will release the mutex and wait on a conditional variable. When this function returns after blocking/waiting, the mutex is locked and acquired by the current thread again. Also, when the function returns, an integer is returned, indicating success(0) or error(error code).

## Q5. Describe Function of `pthread_cond_signal`

According to [man page](https://linux.die.net/man/3/pthread_cond_signal)

> These functions(referring to `pthread_cond_broadcase()` and `pthread_cond_signale`) shall unblock threads blocked on a condition variable.
>
> The `pthread_cond_signal()` function shall unblock at least one of the threads that are blocked on the specified condition variable `cond` (if any threads are blocked on `cond`).
>
> The **thread**(s) that are unblocked shall contend for the mutex according to the scheduling policy (if applicable), and as if each had called `pthread_mutex_lock()`
>
> The `pthread_cond_broadcast()` and `pthread_cond_signal()` functions shall have no effect if there are no threads currently blocked on `cond`

Simply, when `pthread_cond_signal()` is called, at least one at least one of the threads that are blocked on the specified condition variable `cond` is unblocked. And implicitly, all unblocked threads will contend for the mutex specified when previously calling `pthread_cond_wait()`. If there is none blocked on `cond`, then this function shall have no effect. Also, when the function returns, an integer is returned alongside, indicating success(0) or error(error code).

## Q6. Describe Function of `pthread_mutex_unlock`

`pthread_mutex_unlock()` simply unlocks a mutex, making it available. Also, when the function returns, an integer is returned alongside, indicating success(0) or error(error code).

## Q7. Describe Function of `sem_open`

`sem_open()` creates a new `POSIX` semaphore or opens an existing semaphore, both of which are identified by a string `name`. On success, `sem_open()` returns the address (`sem_t*`)of the new semaphore; this address is used when calling other semaphore-related  functions. On  error,  `sem_open()` returns `SEM_FAILED`, with `errno` set to indicate the error.

## Q8. Describe Function of `sem_wait`

`sem_wait()` (implicitly locks then) decrements the specified semaphore. If the semaphore's value is greater than zero, then the decrement proceeds, and the function returns, immediately.  If the semaphore currently has the value zero, then the call blocks until either it becomes possible(i.e. positive) to perform the decrement. Also, it returns 0 on success; on error, the value of the semaphore is left unchanged, -1 is returned, and `errno` is set to indicate the error.

## Q9. Describe Function of `sem_post`

`sem_post()` increments the semaphore If the semaphore's value consequently becomes greater than zero, then another process or thread blocked in a `sem_wait()`call will be woken up and proceed to lock the semaphore. Also, it returns 0 on success; on error, the value of the semaphore is left unchanged, -1 is returned, and `errno` is set to indicate the error.

## Q10. Describe Function of `sem_close`

`sem_close()` simply closes the specified semaphore, allowing to free allocated resources of the semaphore. Also, it returns 0 on success; on error, -1 is returned, and `errno` is set to indicate the error.

## Q11. Producer-Consumer Problem (understand producer_consumer.c): Are the data that consumers read from the buffer produced by the same producer

Yes, because there is only one producer thread, which is generating random integers.

## Q12. Producer-Consumer Problem（understand producer_consumer.c）: What is the order of the consumer's read operations and the producer's write operations, and their relationship

The order of the consumer’s read operations:

1. acquire the mutex
2. check whether the buffer is empty
   * if so, wait for the `empty`
   * else, proceed
3. check whether the buffer is full
   * if so, signal the producer, because after the consumer consumed one item, there will be empty space left.
   * else, proceed
4. consume/pop one item
5. move the head of the buffer to the next place where is not empty
6. release the mutex

The order of the producer’s write operations:

1. acquire the mutex
2. check whether the buffer is full
   * if so, wait for the `full`
   * else, proceed
3. check whether the buffer is empty
   * if so, signal the consumer, because after the producer produced one item, there will be one new item available
   * else, proceed
4. produce/append one item
5. move the tail of the buffer to the next place where is empty
6. release the mutex

Relationship: These operations just like mirroring each other, and they are kind of symmetric. Also, as can be seen in the code, once the buffer is full, the producer will wait and then the next thread to run is definitely the consumer thread, and once the buffer is empty, the consumer will wait and the next thread to run is definitely the producer thread. As for other situations, it is up to the scheduler to manage which thread to run and also up to the thread that first locks the mutex.

## Q13. Producer-Consumer Problem（understand producer_consumer.c）: Briefly describe the result of the program

We can see outputs like the below:

```
[H]<---| C | X | A | F | C |   |   |   |   |<---[ ] //consumer consuming
[C]<---| X | A | F | C |   |   |   |   |   |<---[ ] //consumer consuming
[ ]<---| X | A | F | C | M |   |   |   |   |<---[M]	//producer producing
[ ]<---| X | A | F | C | M | U |   |   |   |<---[U] //producer producing
```

The result is such, indicating that producers and consumers are like at two ends of a pipe, dumping to or taking from the “pipe”. However, the buffer behaves rather like a circular queue, because the `head` is not necessarily the front-most shown in the output, nor the `tail` the rearrest, and the “queue” keeps rotating.

## Q14. Producer-Consumer Problem（understand producer_consumer.c）: What queue is used in this program, and its characteristics?

Circular Queue is used.

Characteristics:

* It is often conceptually circular instead of implementationally, because it is often realized using arrays.
* When it is implemented using arrays, the number of maximum items it can hold is one less than the size of its underlying array
* It has fixed maximum capacity

## Q15. Producer-Consumer Problem（understand producer_consumer.c）: Briefly describe the mutual exclusion mechanism of this program

Basically, the mutual exclusion here is realized by locking a mutex. A mutex can be atomically(i.e. without interruptions) acquired/locked by one and only one thread, and the other thread that did not successfully contend for the mutex will be blocked in `pthread_mutex_lock()` until the mutex is unlocked. So we can ensure that only one thread is executing codes in the critical section(CS) by acquiring/locking the mutex before going into the CS, and thus realize mutual exclusion. 

However, when executing `pthread_cond_wait()`, the mutex is unlocked inside the CS, and then the executing thread is blocked inside the function call. So to ensure the mutual exclusion when the thread is waken up, this thread should contend for the mutex(i.e. try to lock the mutex) right after it is unblocked. This mechanism is implicitly realized by schedulers and libraries, as stated in the [man page of pthread_cond_signal](https://linux.die.net/man/3/pthread_cond_signal):

> The **thread**(s) that are unblocked shall contend for the mutex according to the scheduling policy (if applicable), and as if each had called `pthread_mutex_lock()`

```
boolean[] ca; //chopsticks_available
cond[] conds;
mutex m;

think()
m.lock()
//if need chopstick i and j
while(true)
    if(ca[i]&&ca[j])
        ca[i] = false;
        ca[j] = false;
        break
    else if(!ca[i])
        pthread_cond_wait(cond[i],m)
    else
        pthread_cond_wait(cond[j],m)
m.release()

eat()

m.lock()
ca[i] = true;
ca[j] = true;
signal(cond[i])
signal(cond[j]);
m.release()
        
```



