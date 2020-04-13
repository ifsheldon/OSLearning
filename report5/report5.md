# Report 5

## Q1. What is deadlock

According to [wiki](https://en.wikipedia.org/wiki/Deadlock):

> In [concurrent computing](https://en.wikipedia.org/wiki/Concurrent_computing), a **deadlock** is a state in which each member of a group is waiting for another member, including itself, to take action, such as sending a message or more commonly releasing a [lock](https://en.wikipedia.org/wiki/Lock_(computer_science))

Simply, a deadlock is a situation where components of a system wait for each other and thus the system takes no progress.

## Q2. What are the requirements of deadlock?

* Mutual Exclusion
  * Only one thread can access one resource at a time
* Hold and wait
  * A thread holds available needed resources and waits for unavailable resources
* No preemption
  * Resources can only be released when the thread holding them releases them.
* Circular wait

## Q3. What’s different between deadlock prevention and deadlock avoidance?

Deadlock prevention tries to resolve the problem of deadlocks by breaking the requirements of deadlocks when a deadlock has come up while deadlock avoidance tries to avoid deadlocks using some algorithms before deadlocks come up.

## Q4. How to prevent deadlock? Give at least two examples

One requirement of deadlocks is mutual exclusion, which is needed when accessing shared resources. Therefore, if we can replicate resources, then mutual exclusion is no longer needed, which breaks the prerequisite of deadlocks. One situation is that two threads both need to access two printers at one time. When thread A first acquired printer A and thread B printer B, then a deadlock comes up. One example of the solutions is straight-forward. If the budget permits, we can simply buy two more printers, two for each thread. No threads have to share printers, then the problem is solved.

Another example of the solutions is that we can assign different priorities to these two threads and the one with the higher priority can force the other to give up printers it has, which breaks the prerequisite of no preemption.

## Q5. Which way does recent UNIX OS choose to deal with deadlock problem, why?

Initially, Unix just pretended deadlocks never happen and ignored them because dealing with deadlocks will add a constant overhead, and if deadlocks rarely happen, such overhead is expensive. Recently, according to [Oracle](https://docs.oracle.com/cd/E19455-01/806-4750/fileio-18/index.html), Unix also provides locking facilities to support deadlock prevention and avoidance because of the need to reduce waiting time and data loss in some critical applications.

## Q6. What data structures you use in your implementation（of Banker's algorithm）? Where and why you use them? Are they optimal for your purpose?

|     Data Structure      |                       Where                        |                             Why                              |                         Is optimal?                          |
| :---------------------: | :------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| Dynamic Array(`Vector`) |         Storing the result(`bool` values)          |               can hold various amounts of data               | No, a linked list(`forward_list`) should be better because of (very little) less overhead |
| Hash Table(`hash_map`)  | Storing `need`, `allocated` and `request` matrices | dynamic space; suitable for sparse matrices; small overhead  | In most cases, yes, but not in the case where `pid`s are continuous integers, which means the 3 matrices are dense. |
|          Array          |              Iterating all processes               | When checking safety, processes are intensively accessed. Thus, accessing them via hash tables will bring more overhead, so an array is used. |                             Yes                              |

