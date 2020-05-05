# Report 7

## Q1. Briefly describe the FIFO page-replacement algorithm and analyze its algorithm complexity

The FIFO page-replacement algorithm simply maintains a fixed-size FIFO queue of pages. When this queue is not full, an allocated page is simply added to the tail of the queue. When the queue is full and a page is newly allocated, the page at the front of the queue will be popped out first, the the new page is appended to the queue. There are two ways to implement the FIFO queue, one to use pointers, the other to use an array to implement a cyclic queue. Either way, the time complexity for popping and appending a page is `O(1)`. So for a sequence of length n, the time needed to process it is `n*O(1)=O(n)`. And the space complexity is `O(m), m for the maximum capacity of the queue`.

## Q2. Briefly describe the MIN page-replacement algorithm and analyze its algorithm complexity

MIN page-replacement algorithm is a static algorithm, which means it has to know all the information before running. The pseudo-code of the algorithm is the following.

```python
Given a page allocation sequence seq[] and a fixed-size space cache.
for page in seq[]:
	if cache is not full:
		put page in cache
	else:
		find one page p that satisfies the following requirements:
			* the page number of p is in the collection of page numbers of the pages in cache.
			* p is the latest in seq[], i.e. has the largest index number in seq[]
		find the corresponding page p' in cache that has the same page number of p
		replace p' with page.
```

The time complexity is `O(mn), m for the cache size, and n for the sequence length`. The reason is that firstly we need to go through `n` loops, and inside one loop, the complexity of `if` branch is `O(1)` and that of `else` branch is `O(m)` because the first and the second step of it go through the cache, which is of `O(m)` and the third step is of `O(1)`, then totally the complexity is `O(n)*O(m)=O(mn)`.

The space complexity of this algorithm is always `O(m+n)` in implementations of different time complexities. 

## Q3. Briefly describe the LRU page-replacement algorithm and analyze its algorithm complexity

The idea of LRU page-replacement algorithm is relatively straightforward. That is, when the space is full and a new page is allocated, then replace the least recently used page in memory with the new page. However, there are various implementations, and their time complexities differ from each other.

Hardware Implementations:

| Implementation |              Time Complexity(`m` for capacity)               |
| :------------: | :----------------------------------------------------------: |
|    Counter     | Update: Each counter of each page increments by 1 on every clock tick. So the number of increment operations is `m`, but if each counter has its own adder, the needed time to update all counters is `O(1)`. If a page is hit, its counter is set to 0 with the cost of `O(1)` time<br>Find the LRU page: There are `m` comparisons, but if comparisons are done in a tree manner, `O(m)` comparators is needed and the needed time is `O(log_2m)`<br>Hit: the time complexity depends of associativity of the hardware for storage |
|   Time stamp   | Update: when a page is stored in memory or hit, its time stamp is set with the cost of `O(1)` time.<br>Find the LRU page: There are `m` comparisons, but if comparisons are done in a tree manner, `O(m)` comparators is needed and the needed time is `O(log_2m)`<br>Hit: the time complexity depends of associativity of the hardware for storage |

Software Implementations:

|       Implementation        |              Time Complexity(`m` for capacity)               |
| :-------------------------: | :----------------------------------------------------------: |
| Doubly-linked List+ HashMap | Hit: `O(1)` since it uses a hash map.<br>Find the LRU page: `O(1)` to remove the tail of the linked list.<br>Update/Add: `O(1)` to add the page to the front of the linked list. |
|           Arrays            | Hit: `O(1)` if use page numbers to index the array, or `O(n)` to find a page.<br>Find the LRU page: `O(n)` comparisons.<br>Update: `O(1)` if use time stamps(the same as the hardware time stamp implementation) or `O(n)` if use counters, for there’re `O(n)` increments. |

As for space complexity, it is `O(m)` for all implementations.

## Q4. Briefly describe the clock algorithm and analyze its algorithm complexity

The clock algorithm basically maintains a cyclic queue with a certain size. The pointer of the cyclic queue is the arm of the clock. The pointer moves only when there is a page coming. The pseudo-code is the following.

```python
Init: A cyclic queue with all nodes' flags set to 0 and contents set to null. Pointer ptr is pointing to an initial node.

When a new page p comes in, do the following:
AddPage(Page p):
    loop:
        if ptr->flag == 0:
            if ptr->content != null:
                store ptr->content
            ptr->content = p
            ptr->flag = 1
            break loop
        else:
            ptr->flag = 0
            move ptr to next place

When accessing a page p', do the following:
if p' is in memory:
	p'.flag = 1
else:
	restore p' from disk
	call AddPage(p')
```

The time complexity is `O(m), m for the capacity`, since in the worst case, the pointer must go through all nodes and set their flags to 0 before finding one place to insert a page. For a page sequence of length n, the time needed to process it is `O(m)*n=O(mn)`.

## Q5. Briefly describe the second-chance algorithm and analyze its algorithm complexity

The second-chance algorithm is a combination of FIFO and LRU. The pseudo-code is the following:

```python
Given a cache size n, then the size of FIFO cache is n/2, the size of LRU cache is (n - n/2).
Given a page sequence seq[]
for page in seq[]:
	if page is in cache:
		if page is in FIFO cache:
			do nothing
		else:
			move page to FIFO queue as if it is a new page.
			after moving one page pre is popped out by FIFO queue.
			put pre into LRU queue as if it is a new page.
	else:
		put page into FIFO queue.
		if there is one page pre popped out by FIFO:
			put pre into LRU queue as if it is a new page.
```

Since it is a combination of FIFO and LRU, the time complexity of all operations is `O(1)`. Then for a page sequence of n items, the time needed is `n*O(1) = O(n)`. The space complexity is `O(m), m for the cache size`.