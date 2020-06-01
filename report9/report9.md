# Report 9

Assuming that, for all of the questions in this part, filesystem blocks are 4 KBytes.

## Q1. Consider a really simple filesystem, **cs302fs**, where each inode only has 10 direct pointers, each of which can point to a single file block. Direct pointers are 32 bits in size (4 bytes). What is the maximum file size for **cs302fs**?

Since there are only 10 direct pointers, the number of file blocks that an inode can point to is 10. Therefore, the maximum file size for **cs302fs** is 40KB. 

## Q2. Consider a filesystem, called **extcs302fs**, with a construct called an extent. Extents have a pointer (base address) and a length (in blocks). Assume the length field is 8 bits (1 byte). Assuming that an inode has exactly one extent. What is the maximum file size for **extcs302fs**?

Since the length field has 8 bits, the maximum number it can represent is 255. Therefore, an inode of **extcs302fs** can address a continuous space of the maximum size of 255 blocks, then the maximum file size is 255*4KB = 1020KB.

## Q3. Consider a filesystem that uses direct pointers, but also adds indirect pointers and double-indirect pointers. We call this filesystem, **indcs302fs**. Specifically, an inode within **indcs302fs** has 1 direct pointer, l indirect pointer, and 1 doubly-indirect pointer field. Pointers, as before, are 4 bytes (32 bits) in size. What is the maximum file size for **indcs302fs**?

The number of blocks that an inode can point to is 1+(4KB/4B)+(4KB/4B)^2, then the maximum file size is 4KB*(1+(4KB/4B)+(4KB/4B)^2) = 4KB+4MB+4GB = 4198404B