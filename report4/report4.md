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

## Q11. Producer-Consumer Problem (understand producer_consumer.c): Are the data that consumers read from the buffer are produced by the same producer



## Q12. Producer-Consumer Problem（understand producer_consumer.c）: What is the order of the consumer's read operations and the producer's write operations, and their relationship

## Q13. Producer-Consumer Problem（understand producer_consumer.c）: Briefly describe the result of the program

## Q14. Producer-Consumer Problem（understand producer_consumer.c）: What queue is used in this program, and its characteristics?

## Q15. Producer-Consumer Problem（understand producer_consumer.c）: Briefly describe the mutual exclusion mechanism of this program

