# Report 7

## Q1. Briefly describe the FIFO page-replacement algorithm and analyze its algorithm complexity

The FIFO page-replacement algorithm simply maintains a fixed-size FIFO queue of pages. When this queue is not full, an allocated page is simply added to the tail of the queue. When the queue is full and a page is newly allocated, the page at the front of the queue will be popped out first, the the new page is appended to the queue. There are two ways to implement the FIFO queue, one to use pointers, the other to use an array to implement a cyclic queue. Either way, the time complexity for popping and appending a page is $O(1)$. And the space complexity is $O(m),\ m\ for\ the\ maximum\ capacity\ of\ the\ queue$.

## Q2. Briefly describe the MIN page-replacement algorithm and analyze its algorithm complexity

MIN page-replacement algorithm is a static algorithm, which means it has to know all the information before running. The pseudo-code of the algorithm is the following.

```
Given a page allocation sequence seq[] and a fixed-size space mem.
for page in seq[]:
	if mem is not full:
		put page in mem
	else:
		find one page p that satisfies the following requirements:
			* the page number of p is in the collection of page numbers of the pages in mem.
			* p is the latest in seq[], i.e. has the largest index number in seq[]
		find the corresponding page p' in mem that has the same page number of p
		replace p' with page.
```

The time complexity of this algorithm can be $O(nm),\ n\ for\ the\ length\ of\ seq[]\ and\ m\ for\ the\ capacity\ of\ mem,\ presumably\ n>m$ if this algorithm is implemented badly. The reason is that we need to at most go through $n$ pages, and to check whether the page number is in mem, we need to at most go through $m$ pages, so the time complexity of the first step in `else` is $O(n*m)$, and those of the second and the third step are $O(m)$, $O(1)$, respectively. However, if implemented properly, this algorithm can be of $O(n)$ time complexity, since we can reduce the time complexity of checking whether a page number is in mem to $O(1)$ by hashing or directly indexing a boolean array with page numbers. 

The space complexity of this algorithm is always $O(m+n)$ in implementations of different time complexities. 

## Q3. Briefly describe the LRU page-replacement algorithm and analyze its algorithm complexity

The idea of LRU page-replacement algorithm is relatively straightforward. That is, when the space is full and a new page is allocated, then replace the least recently used page in memory with the new page. However, there are various implementations, and their time complexities differ from each other.

Hardware Implementations:

| Implementation |              Time Complexity($m$ for capacity)               |
| :------------: | :----------------------------------------------------------: |
|    Counter     | Update: Each counter of each page increments by 1 on every clock tick. So the number of increment operations is $m$, but if each counter has its own adder, the needed time to update all counters is $O(1)$. If a page is hit, its counter is set to 0 with the cost of $O(1)$ time<br>Find the LRU page: There are $m$ comparisons, but if comparisons are done in a tree manner, $O(m)$ comparators is needed and the needed time is $O(log_2m)$<br>Hit: the time complexity depends of associativity of the hardware for storage |
|   Time stamp   | Update: when a page is stored in memory or hit, its time stamp is set with the cost of $O(1)$ time.<br>Find the LRU page: There are $m$ comparisons, but if comparisons are done in a tree manner, $O(m)$ comparators is needed and the needed time is $O(log_2m)$<br>Hit: the time complexity depends of associativity of the hardware for storage |

Software Implementations:

|       Implementation        |              Time Complexity($m$ for capacity)               |
| :-------------------------: | :----------------------------------------------------------: |
| Doubly-linked List+ HashMap | Hit: $O(1)$ since it uses a hash map.<br>Find the LRU page: $O(1)$ to remove the tail of the linked list.<br>Update/Add: $O(1)$ to add the page to the front of the linked list. |
|           Arrays            | Hit: $O(1)$ if use page numbers to index the array, or $O(n)$ to find a page.<br>Find the LRU page: $O(n)$ comparisons.<br>Update: $O(1)$ if use time stamps(the same as the hardware time stamp implementation) or $O(n)$ if use counters, for there’re $O(n)$ increments. |

As for space complexity, it is $O(m)$ for all implementations.

## Q4. Briefly describe the clock algorithm and analyze its algorithm complexity

The clock algorithm basically maintains a cyclic queue with a certain size. The pointer of the cyclic queue is the arm of the clock. The pointer moves only when there is a page coming. The pseudo-code is the following.

```
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

The time complexity is $O(m),\ m\ for\ the\ capacity$, since in the worst case, the pointer must go through all nodes and set their flags to 0 before finding one place to insert a page.

## Q5. Briefly describe the second-chance algorithm and analyze its algorithm complexity


