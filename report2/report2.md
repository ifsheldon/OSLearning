# Report 2

[TOC]

## Q2 What is system call

According to [Wikipedia](https://en.wikipedia.org/wiki/System_call)

> In [computing](https://en.wikipedia.org/wiki/Computing), a **system call** is the programmatic way in which a [computer program](https://en.wikipedia.org/wiki/Computer_program) requests a service from the [kernel](https://en.wikipedia.org/wiki/Kernel_(computing)) of the [operating system](https://en.wikipedia.org/wiki/Operating_system) it is executed on.

Simply, it is a function call provided by OS for programs to get a service from the kernel of OS.

## Q3 What is fork

The meaning of “fork” of OS derives from “fork” of a road which means a type of intersection where a road splits. 

According to [Wikipedia](https://en.wikipedia.org/wiki/Fork_(system_call))

> **fork** is an operation whereby a [process](https://en.wikipedia.org/wiki/Computer_process) creates a copy of itself.

And its function is

> In multitasking operating systems, processes (running programs) need a way to create new processes, e.g. to run other programs. Fork and its variants are typically the only way of doing so in Unix-like systems.
>
> The fork operation creates a separate [address space](https://en.wikipedia.org/wiki/Address_space) for the child. The child process has an exact copy of all the memory segments of the parent process.

Simply, `fork` is a system call, by using which programmers can create new processes.

## Q4 How to realize inter-process communication

Summarized from [Wikipedia](https://en.wikipedia.org/wiki/Inter-process_communication#Approaches), inter-process communication (IPC) can be realized by the following approaches.

|                            Method                            |                      Short Description                       |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|     [File](https://en.wikipedia.org/wiki/Computer_file)      | A record stored on disk, or a record synthesized on demand by a file server, which can be accessed by multiple processes. |
|                     Communications file                      |            A unique form of IPC in the late-1960s            |
| [Signal](https://en.wikipedia.org/wiki/Signal_(computing)); also [Asynchronous System Trap](https://en.wikipedia.org/wiki/Asynchronous_System_Trap) | A system message sent from one process to another, not usually used to transfer data but instead used to remotely command the partnered process. |
|    [Socket](https://en.wikipedia.org/wiki/Network_socket)    | Data sent over a network interface, either to a different process on the same computer or to another computer on the network. Stream-oriented ([TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol); data written through a socket requires formatting to preserve message boundaries) or more rarely message-oriented ([UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol), [SCTP](https://en.wikipedia.org/wiki/SCTP)). |
| [Unix domain socket](https://en.wikipedia.org/wiki/Unix_domain_socket) | Similar to an internet socket, but all communication occurs within the kernel. Domain sockets use the file system as their address space. Processes reference a domain socket as an [inode](https://en.wikipedia.org/wiki/Inode), and multiple processes can communicate with one socket |
| [Message queue](https://en.wikipedia.org/wiki/Message_queue) | A data stream similar to a socket, but which usually preserves message boundaries. Typically implemented by the operating system, they allow multiple processes to read and write to the [message queue](https://en.wikipedia.org/wiki/Message_queue) without being directly connected to each other. |
| [Anonymous pipe](https://en.wikipedia.org/wiki/Anonymous_pipe) | A unidirectional data channel using [standard input and output](https://en.wikipedia.org/wiki/Stdin). Data written to the write-end of the pipe is buffered by the operating system until it is read from the read-end of the pipe. Two-way communication between processes can be achieved by using two pipes in opposite "directions". |
|    [Named pipe](https://en.wikipedia.org/wiki/Named_pipe)    | A pipe that is treated like a file. Instead of using standard input and output as with an anonymous pipe, processes write to and read from a named pipe, as if it were a regular file. |
| [Shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)) | Multiple processes are given access to the same block of [memory](https://en.wikipedia.org/wiki/Memory_(computing)) which creates a shared buffer for the processes to communicate with each other. |
| [Message passing](https://en.wikipedia.org/wiki/Message_passing) | Allows multiple programs to communicate using message queues and/or non-OS managed channels. Commonly used in concurrency models. |
| [Memory-mapped file](https://en.wikipedia.org/wiki/Memory-mapped_file) | A file mapped to [RAM](https://en.wikipedia.org/wiki/RAM) and can be modified by changing memory addresses directly instead of outputting to a stream. This shares the same benefits as a standard [file](https://en.wikipedia.org/wiki/File_(computing)). |

## Q5 How to realize inter-process connection

It seems there is no definition for inter-process connection. Therefore, connection-based and channel-based inter-process communication are presumed. Approaches of connection-based and channel-based inter-process communication are the followings, which are typically realized by sockets, pipes or message queues. The table is summarized from [Wikipedia Inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication#Approaches)

|                            Method                            |                      Short Description                       |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|    [Socket](https://en.wikipedia.org/wiki/Network_socket)    | Data sent over a network interface, either to a different process on the same computer or to another computer on the network. Stream-oriented ([TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol); data written through a socket requires formatting to preserve message boundaries) or more rarely message-oriented ([UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol), [SCTP](https://en.wikipedia.org/wiki/SCTP)). |
| [Unix domain socket](https://en.wikipedia.org/wiki/Unix_domain_socket) | Similar to an internet socket, but all communication occurs within the kernel. Domain sockets use the file system as their address space. Processes reference a domain socket as an [inode](https://en.wikipedia.org/wiki/Inode), and multiple processes can communicate with one socket |
| [Message queue](https://en.wikipedia.org/wiki/Message_queue) | A data stream similar to a socket, but which usually preserves message boundaries. Typically implemented by the operating system, they allow multiple processes to read and write to the [message queue](https://en.wikipedia.org/wiki/Message_queue) without being directly connected to each other. |
| [Anonymous pipe](https://en.wikipedia.org/wiki/Anonymous_pipe) | A unidirectional data channel using [standard input and output](https://en.wikipedia.org/wiki/Stdin). Data written to the write-end of the pipe is buffered by the operating system until it is read from the read-end of the pipe. Two-way communication between processes can be achieved by using two pipes in opposite "directions". |
|    [Named pipe](https://en.wikipedia.org/wiki/Named_pipe)    | A pipe that is treated like a file. Instead of using standard input and output as with an anonymous pipe, processes write to and read from a named pipe, as if it were a regular file. |
| [Message passing](https://en.wikipedia.org/wiki/Message_passing) | Allows multiple programs to communicate using message queues and/or non-OS managed channels. Commonly used in concurrency models. |

Specifically, in Unix-like OSs, `stdin` and `stdout` can be connected via pipes `|` that are implemented by OSs.

## Q6 Write the prototype of function `fork`

According to [man.org](http://man7.org/linux/man-pages/man2/fork.2.html), the prototype of `fork` of Linux in C is 

```c
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
```

## Q7 Write the prototype of function `signal`

According to [man.org](http://man7.org/linux/man-pages/man2/signal.2.html), the prototype of `signal` of Linux in C is 

```c
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

## Q8 Write the prototype of function `pipe`

According to [man.org](http://man7.org/linux/man-pages/man2/pipe.2.html), the prototype of `pipe` of Linux in C is 

```c
#include <unistd.h>
/* On Alpha, IA-64, MIPS, SuperH, and SPARC/SPARC64; see NOTES */
struct fd_pair {
    long fd[2];
};
struct fd_pair pipe();
/* On all other architectures */
int pipe(int pipefd[2]);
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>
int pipe2(int pipefd[2], int flags);
```

## Q9 Write the prototype of function `tcsetpgrp`

According to [man.org](http://man7.org/linux/man-pages/man3/tcgetpgrp.3.html), the prototype of `tcsetpgrp` of Linux in C is

```c
#include <unistd.h>
pid_t tcgetpgrp(int fd);
int tcsetpgrp(int fd, pid_t pgrp);
```

## Q10 Execute `fork.c` and observe, please describe the result (not execution result)

The string result of `printf("argc = %d, argv[0] = %s", argc, argv[0])` has not been printed out on the terminal while the result of `execvp(prog_argv[0],prog_argv)` has been output.

The explanation is kind of complex, which includes two aspects, i.e. I/O buffering and the behavior of `exec`.

According to [Wikipedia](https://en.wikipedia.org/wiki/Exec_(system_call)), the system calls in `exec` family clean up user-level data of a process while kernel-level data of it remains unchanged.

> As a new process is not created, the [process identifier](https://en.wikipedia.org/wiki/Process_identifier) (PID) does not change, but the [machine code](https://en.wikipedia.org/wiki/Machine_code), [data](https://en.wikipedia.org/wiki/Data_(computing)), [heap](https://en.wikipedia.org/wiki/Heap_(programming)), and [stack](https://en.wikipedia.org/wiki/Run-time_stack) of the process are replaced by those of the new program.

As for buffering, according to a [post](http://www.pixelbeat.org/programming/stdio_buffering/), as illustrated in the below figure,

 ![buffering problem in unix shell pipeline](http://www.pixelbeat.org/programming/stdio_buffering/pipe_stdio_example.png)

`stdin` and `stdout` are buffered at the user level, i.e. within user space. And according to [another post](https://eklitzke.org/stdout-buffering), `stdout` is line-buffered if it is TTY, i.e. the open file descriptor is referring to a terminal, which is the case when we run `fork.c` using shells.

With these following premises:

* `stdout` is buffered at the user level
* `stdout` is line-buffered
* `exec` will clean up all user-level data

we can easily conclude that as `printf("argc = %d, argv[0] = %s", argc, argv[0])` does not output a return carriage `\n`, the string will not be output immediately and be buffered at user level, which is then cleaned up by `exec`(specifically `execvp` in this case). Therefore, the string will not be output to shells.

## Q11 Execute `fork.c` and observe, please describe how to distinguish between parent and child processes in a program

We can use the return value of `fork()` to distinguish between parent and child processes. When child processes return from `fork()`, they get the return value 0. However, when a parent process returns from `fork()`, it usually gets a positive return value, which is the pid of its child, if `fork()` has been successfully executed. If not, the parent will get a negative value indicating execution failures.

## Q12 Execute `pipe.c` and observe, please describe the result (not execution result)

The result can be explained in a quite straightforward way using the diagram below which is self-explanatory.

![img](https://raw.githubusercontent.com/ifsheldon/OSLearning/master/report2/Process.png)

The picture may be loaded slowly because the Great Wall blocked some content servers of Github. If the picture does not show up, please click this [link](![img](https://raw.githubusercontent.com/ifsheldon/OSLearning/master/report2/Process.png)) (may need a VPN).

What is noteworthy is that under extreme cases, if `stdout` has not been redirected to `pipe in` in Process C when Process B executes `ls`, the output of `ls` may be forwarded to the shell instead of the pipe.

## Q13 Execute `pipe.c` and observe. Is `execvp(prog2_argv[0],prog2_argv)//(Line 56)` executed? And why?

Yes, it is executed because `execvp(prog2_argv[0],prog2_argv)` is telling the kernel to execute the command`more`, which is in fact executed when the program runs.

## Q14 Execute `signal.c` and observe, please describe the result (not execution result)

When running `signal.c`, there are two processes, and their sigaction handlers are both `void ChildHandler(int sig, siginfo_t* sip, void* notused)`. If we just run the program normally, what we can see just two processes output their PIDs and run forever. If we use `kill` to send signals to these processes, we can see different results depending on various signals we sent. The results are below:

| Signal Receiver | SIGNAL                                                       | Output                                                       |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Child process   | `SIGALRM` , `SIGKILL` or other signals that cause the process to exit | "The process generating the signal is PID: %d\n"<br>"The child is gone!!!!!\n" |
| Child process   | `SIGSTOP`, `SIGCHLD` or other signals that do not cause the process to exit | "The process generating the signal is PID: %d\n"<br/>"Uninteresting\n" |
| Parent process  | all signals                                                  | "The process generating the signal is PID: %d\n"             |

This is because `sa_sigaction` is bound to `void ChildHandler(int sig, siginfo_t* sip, void* notused)`, when a signal comes in, this handler will be called, and `void ChildHandler(int sig, siginfo_t* sip, void* notused)` is also bound to the handler of `SIGCHLD`, so when a `SIGCHLD` signal comes in, it will also be called.

## Q15 Execute `signal.c` and observe. Please answer, how to execute function ChildHandler?

We can use `kill` to send signals to processes and then trigger/call the signal handler, which in this case is `ChildHandler`. As for different signals, we can use the arguments of `kill` to send various signals, such as `SIGKILL` and `SIGCHLD`.

## Q16 Execute `process.c` and observe, please describe the result (not execution result)

The program contains a minor bug, the change made is shown below.

```c
// before
if (!cpid)
{
    fprintf(stdout,"ID(child)=%d\n",getpid());
    /* 使子进程所在的进程组成为前台进程组，然后执行vi */
    setpgid(0,0);
    tcsetpgrp(0,getpid());
    execvp("/bin/vi","vi",NULL);
    exit(-1);
}

// after
if (!cpid)
{
    fprintf(stdout,"ID(child)=%d\n",getpid());
    /* 使子进程所在的进程组成为前台进程组，然后执行vi */
    setpgid(0,0);
    tcsetpgrp(0,getpid());
    char* argv[]={"vi",NULL};
    execvp("/usr/bin/vi", argv);
    exit(-1);
}
signal(SIGTTOU, SIG_IGN);
```

When the program begin to run, `vi` will be shown on the shell, and after quitting from `vi`, the program enter an infinite loop, taking input from the shell and echo back on the output of shell. The reason of this output is simple. The program first forks two processes, and the child process execute `vi`, taking the input from shell first, while the parent process waits for the child process to exit. After the child process exits, the parent process take the shell’s input and enter a loop, echoing users’ input.

## Q17 Execute `process.c` and observe. Please answer, how many ./process in the process list? And what’s the difference between them?

There’s only one `./process` in the process list, and `vi`, which is the child process of it, is also in the list.

## Q18 Execute `process.c` and observe. Please answer, what happens after killing the main process

After killing the main process, the child process `vi` exits and outputs error messages.