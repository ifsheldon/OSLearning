# Q1

`mkdir` is for making a new directory. The passed argument is `~/11712005`, which means the path is in the folder of the current user, e.g. `/home/current_username/1172005/` or `/root/11712005/` (if the current user is `root`). In my case, it is `/home/ubuntu/11712005/`.

![image-20200221155532363](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221155532363.png)

# Q2

`ls` is for listing directory contents. The option `-la` is short for `-l -a`, of which meanings are using a long list format and all(not ignoring entries starting with `.`), respectively. The passed argument is `~`, which is the path of `/home/current_user/` or or `/root/` (if the current user is `root`). In my case, it is `/home/ubuntu/`.

![image-20200221155707596](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221155707596.png)

# Q3

![image-20200221160658713](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221160658713.png)

`cd` is for changing directories. The passed argument is `~/11712005`, which is the path of the folder of the current user, e.g. `/home/current_username/1172005/` or `/root/11712005/` (if the current user is `root`), which, in my case, is `/home/ubuntu/11712005/`.

# Q4

`man` is “an interface to the on-line reference manuals”, which can look up a command’s manual.  The passed argument is `grep`, which is the name of the command.

![image-20200221161428913](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221161428913.png)

# Q5

`mv` is for moving(renaming) files. The passed arguments are `~/11712005` and `/home`, the first of which is the folder of `/home/current_user/11712005`(in my case, it is `/home/ubuntu/11712005`). The command is to move the directory of `/home/ubuntu/11712005` into `/home/`.

![image-20200221162053151](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221162053151.png)

![image-20200221162650181](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221162650181.png)

# Q6

`rm` is for removing files or directories. The option `-r` means removing recursively, which is needed when deleting all of the contents in a directory. The passed argument is the path of the directory `/home/11712005`.

![image-20200221162802260](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221162802260.png)

# Q7

`cp` is for copying files and directories. The first argument is `/etc/apt/sources.list`, which is the source, and the second one is `/etc/apt/sources.list.bak`, which is the destination.![image-20200221164538245](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221164538245.png)

# Q8

`cat` is for concatenating files and print on the standard output. The argument is the path of a file `/etc/shells`. This command here actually just print out the content of the file.

![image-20200221165232451](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221165232451.png)

# Q9

`cat` is for concatenating files and print on the standard output. The argument of `cat` is the path of a file `/etc/shells`. `grep` is for printing lines matching a pattern, which here is `bash`, meaning containing the string of “bash”. `|` is for tube or tunnel. Here it means taking the standard output of `cat` as the standard input of `grep`. This command in fact goes through the content of `/etc/shells`, searching for lines containing “bash” as substrings.

![image-20200221170148293](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221170148293.png)

# Q10

To find out the `pid`s of two terminals, we can find their associated shells’ `pid`s. The default shell I use is `bash`, so I type `ps -ef|grep bash`. As I am connecting a remote machine via `ssh`, to kill one of the terminals, I can kill its process or its parent process(which is the process of `sshd`). To kill its process, type `kill 29688` and the corresponding terminal is closed.

![image-20200221172643182](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221172643182.png)

# Q11

Source Code:

```c
#include <stdio.h>
int main()
{
    int x = 0;
    x+=1;
    x+=1;
    x+=1;
    printf("%d\n",x);
    return 0;
}
```
Commands:

```bash
gcc -S -O0 opt.c
gcc -S -O1 -o opt.s1 opt.c
```

Results:

```assembly
# in opt.s using O0
        .file   "opt.c"
        .text
        .section        .rodata
.LC0:
        .string "%d\n"
        .text
        .globl  main
        .type   main, @function
main:
.LFB0:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        subq    $16, %rsp
        movl    $0, -4(%rbp)
        addl    $1, -4(%rbp)
        addl    $1, -4(%rbp)
        addl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        movl    %eax, %esi
        leaq    .LC0(%rip), %rdi
        movl    $0, %eax
        call    printf@PLT
        movl    $0, %eax
        leave
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
.LFE0:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
        .section        .note.GNU-stack,"",@progbits
        
# in opt.s1 using O1
        .file   "opt.c"
        .text
        .section        .rodata.str1.1,"aMS",@progbits,1
.LC0:
        .string "%d\n"
        .text
        .globl  main
        .type   main, @function
main:
.LFB23:
        .cfi_startproc
        subq    $8, %rsp
        .cfi_def_cfa_offset 16
        movl    $3, %edx
        leaq    .LC0(%rip), %rsi
        movl    $1, %edi
        movl    $0, %eax
        call    __printf_chk@PLT
        movl    $0, %eax
        addq    $8, %rsp
        .cfi_def_cfa_offset 8
        ret
        .cfi_endproc
.LFE23:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
        .section        .note.GNU-stack,"",@progbits
```



![image-20200221184221053](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221184221053.png)

![image-20200221184351078](C:\Users\Maple\AppData\Roaming\Typora\typora-user-images\image-20200221184351078.png)

Analysis:

As can be seen in these two screenshots, the numbers of assembly codes are different. The number of instructions of `opt.s1` is less than that of `opt.s`, which means that from the perspective of the number of assembly instructions, O1 optimization will give better performance than O0 optimization. If we take a closer look at the differences, it is noticeable that `addl` instructions are removed from the 01-optimized version of code, which means O1 optimization will examine unnecessary calculations and replace them with the result of these calculations. 