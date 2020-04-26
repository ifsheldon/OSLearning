# Report 6

## Q1. What is Uniprogramming? What is the shortcoming of Uniprogramming?

Uni-programming describes a paradigm in which a computer can have only one task at a time in the main memory. The drawbacks of it are:

* A task will be executed only after the current running task has finished, which means if the current task never ends, other tasks will never make progress.
* Low CPU efficiency because usually tasks need to deal with IO, which is when the CPU left idle. In uni-programming, the CPU is left idle as it is, which decreases the efficiency. For the similar reason, the IO efficiency of uni-programming is also low.

## Q2. What is Multiprogramming? What is the shortcoming of Multiprogramming?

Multi-programming is a rudimentary form of multi-tasking, in which several programs are loaded into the main memory, and each program takes turn to run for some time based on some policies.

The drawbacks are:

* Added overhead of swapping processes, virtualization and protection.
* Added semantic complexity for programmers, i.e. programmers need to understand the semantics of multiprogramming.
* A program cannot fully utilize all resources, which may lead to lower performance.
* Added complexity to OS, e.g. schedulers.

## Q3. What is the segmentation mechanism and its advantages & disadvantages?

Mechanism(Simple Segmentation without Paging):

Information of where the segment is located in memory(the segment base) is associated with each segment. When a program references a memory location by a virtual address, the highest several bits of the virtual address are used to index the segment table, and retrieve the segment base. Then the rest bits are the offset that is to be added to the segment base to give the physical address.

Advantages:

* The overhead is relatively small.
* The size of each segment is flexible.
* It can be implemented by hardwares to speed up

Disadvantages:

* Lead to external fragmentation
* Need continuous blocks of memory
* On a system using segmentation without paging requires that entire segments be swapped back and forth between main memory and secondary storage

## Q4. What is the paging mechanism and its advantages & disadvantages

Mechanism(Simple Single-level Paging):

In the scheme of paging, pages are same-size blocks of storage. Pages can be in the main memory or on the secondary storage(e.g. disks). The attributes of a process are store usually in multiple pages in the main memory. 

There is one page table per process with virtual page numbers as indices for storing the information of the physical page numbers. When a process accesses memory using a virtual address, the highest few bits are as a page index for retrieving the corresponding physical page number, left bits as the offset. 

After validation, the translated memory address from the physical page number is added with the offset, yielding the needed physical address. Moreover, physical pages can be on secondary storage, so when accessed, it will be loaded into the main memory first.

Advantages:

* Solved the problem of external fragmentation
* As same-sized, pages are easily managed by OS
* Enable OS to use secondary storage to let programs exceed the size of available physical memory

Disadvantages:

* If sized inappropriately, pages can contain many internal fragments
* Simple single-level paging may produce a large page table, which is wasteful when the memory occupation of a process is sparse
* Sharing an object(e.g. a linked list) that spans across pages is difficult

## Q5. How to reduce the occurrence of internal fragmentation when allocating memory? Briefly explain why this method works

To reduce the occurrence of internal fragmentation, finer-grain segmentation or paging is needed. For example, we can shrink the sizes of pages or segments. Since the size of each page or segment is smaller, the biggest possible internal fragment is smaller. Therefore, the chance when the size of allocated space cannot be exactly divided by the size of pages or segments(i.e. internal fragmentation occurs) is smaller, although rigorously speaking, this statement is not always hold true and dependent on the distribution of the sizes of allocated spaces. A straightforward but extreme instance is that we take the size of pages or segments to be 1 byte, then there will be no internal fragmentation.

## Q6. How to reduce the occurrence of external fragmentation when allocating memory? Briefly explain why this method works

Paging can fully solve the problem of external fragmentation. This method(simple paging) evenly split the memory into pages with the same size, so there is no empty space(i.e. external fragmentation) left.	

