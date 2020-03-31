# Report 3

## 1. What is a process? What is a program? And what is the difference?

According to [wikipedia](https://en.wikipedia.org/wiki/Process_(computing))

> A **process** is the [instance](https://en.wikipedia.org/wiki/Instance_(computer_science)) of a [computer program](https://en.wikipedia.org/wiki/Computer_program) that is being executed by one or many threads

> A **computer program** is a collection of [instructions](https://en.wikipedia.org/wiki/Instruction_set) that can be [executed](https://en.wikipedia.org/wiki/Execution_(computing)) by a [computer](https://en.wikipedia.org/wiki/Computer) to perform a specific task

> While a computer program is a passive collection of [instructions](https://en.wikipedia.org/wiki/Instruction_set), a process is the actual execution of those instructions

In summary, a process is a running program, and a program is a series of instructions. The difference between them is that a process is an executing dynamic instance that has dynamic states(e.g. resources, attributes) while a program just a static context in which states are just conceptual placeholders.

## 2. What is job? And what is the difference between process and job?

According to [wikipedia](https://en.wikipedia.org/wiki/Job_(computing))

> In [computing](https://en.wikipedia.org/wiki/Computing), a **job** is a unit of work or unit of execution (that performs said work). A component of a job (as a unit of work) is called a *[task](https://en.wikipedia.org/wiki/Task_(computing))* or a *step* (if sequential, as in a [job stream](https://en.wikipedia.org/wiki/Job_stream)). As a unit of execution, a job may be concretely identified with a single [process](https://en.wikipedia.org/wiki/Process_(computing)), which may in turn have subprocesses which perform the tasks or steps that comprise the work of the job; or with a [process group](https://en.wikipedia.org/wiki/Process_group); or with an abstract reference to a process or process group

Simply, jobs are a general concept that represent a unit of work or execution and a job may contain multiple processes while a process is a concrete specification of a job, which has an actual implementation.

## 3. What are the differences between job scheduling and process scheduling?

The granularities of job scheduling and process scheduling differ. As the definition of jobs implies, a job may be concretized by a process group which has multiple processes. Therefore, sometimes, scheduling a job may imply scheduling multiple processes. Sometimes, scheduling a job means picking work to do in the storage, loading them into memory and creating processes while scheduling processes means picking processes in the ready queue to run.

However, if a job is concretized by a standalone process, then from the view of implementation, job scheduling is equivalent to process scheduling, though conceptually a process is a specification of a job. 

## 4. What are the similarities and differences among structures, classes, and unions

### Similarities

Conceptual Similarities:

* they all store states/attributes
* they all allow states/attributes of different types

Implementation Similarities:

* classes and unions can be realized by structures

Practical Similarities:

* these are available in `C++`
* when representing entities of certain types, some languages (e.g. `Java`, `C++`) use classes while some (e.g. `Rust`) use structures

### Differences

Conceptual Differences:

* Obviously their names differ.
* structures and unions are commonly used in imperative languages (e.g. `C`), whose instances represent a block of states/attributes/information to be processed, while classes are usually used in object-oriented languages (e.g. `Java`), whose instances represent a state machine with states, transitions and transition functions (i.e. methods).
* structures represent a record or a block of nested information of possibly different types while unions represent a block of information with possibly different interpretations.

Implementation Difference:

* Each attribute/state in classes and structures has separate space of store, while that in unions shares same space of others. 
* An instance of a class contains implicit pointers to its methods(if the class has methods) while that of a structure or union does not contain pointers to methods because there are no methods.

Practical Differences:

* Writing to a field of a class or a structure does not overwrite the values of other fields while it is not the case in unions
* Generally speaking, structures and unions have less overhead than classes.

## 5. How many statuses are in a job(in the code for this report—scheduler)? And what are they?

There are 3 kinds of statuses, which are `READY`, `RUNNING`, `DONE`, meaning a job ready to run, executing and having finished running, respectively.

## 6. What programs for job control are used in this experiment? And their function?

|   Program   |                           Function                           |
| :---------: | :----------------------------------------------------------: |
|    deq.c    |        As a commander, ask scheduler to dequeue a job        |
|    enq.c    |        As a commander, ask scheduler to enqueue a job        |
|   stat.c    |     As a commander, ask scheduler to show status of jobs     |
| scheduler.c | As a scheduler, schedule jobs to run, suspend and terminate, and also display status of jobs |

## 7. What is used for inter-process communication in this experiment? And its function?

|   IPC Approach   |                           Function                           |
| :--------------: | :----------------------------------------------------------: |
| Named Pipe(FIFO) | For job controllers(`deq.c`, `enq.c`, `stat.c`) to pass commands to `scheduler.c` |
|      Signal      | For parent and child processes to interchange messages and commands |

## 8. What should be noted when printing out the job name

We need to modify `printf( "JID\tPID\tOWNER\tRUNTIME\tWAITTIME\tCREATTIME\tSTATE\n");` to add titles. Also, we need to modify the following codes that contain `printf`

```c
if (current) {
		// str(current->job->create_time)
		strcpy(timebuf,ctime(&(current->job->create_time)));
		timebuf[strlen(timebuf) - 1] = '\0';
		printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\n", //need to add %s for outputting name
			current->job->jid,
			current->job->pid,
			current->job->ownerid,
			current->job->run_time,
			current->job->wait_time,
			timebuf,
			"RUNNING" );
	}
	//iterate through all the linked list
	for (p = head; p != NULL; p = p->next) {
		strcpy (timebuf,ctime(&(p->job->create_time)));
		timebuf[strlen(timebuf) - 1] = '\0';		
		printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\n",//need to add %s for outputting name
			p->job->jid,
			p->job->pid,
			p->job->ownerid,
			p->job->run_time,
			p->job->wait_time,
			timebuf,
			"READY" );
	}
```

## 9. Submit a job that takes more than 100 milliseconds to run (pleas paste your code)



```c
#include<unistd.h>
int main()
{
    while(1)
        usleep(10000);//sleep 10ms
    return 0;
}
```

## 10. List the bugs you found and your fix (Describe the cause of bugs in detail and how to modify it)

1. Chinese Semicolon
```c
   //in scheduler.c
   void updateall()
   {
   	struct waitqueue *p;
   	
   	/* update running job's run_time */
   	if (current)
   		current->job->run_time += 1;   
   	
   	/* update ready job's wait_time */
   	for (p = head; p != NULL; p = p->next) {
   		p->job->wait_time += 1；//chinese semicolon here, just change it to english semi colon
   	}
   }
```

2. The bug of mistaking the unit of `timeval.tv_usec`. The intended duration of time slices is `100ms` while in the code `interval.tv_usec=100;` in the `main()` of `scheduler.c`, it was set to 100 microseconds. To correct this, just modify the code to `interval.tv_usec=100*1000;`

3. The bugs of logic that leads to the loss of elements in the linked list of jobs.

   ```c
   struct waitqueue* jobselect(){
   	/*
       other codes
       */
   	if (head) {
           /*
           other codes
           */
   		if (select == selectprev) head = NULL; // logic flaw here
   	}
   	return select;
   }
   ```

   The code above does not consider the corner case that the job with the highest priority is the head of the linked list. In such case, we should take the head off the linked list, but previously it was mistaken to the case where the linked list has only one node so the head was set to `NULL`. If we do not fix this, when the job with the highest priority is the head of the linked list, we would loss the entire linked list. The same logic flaw occurred in `do_deq()`.

   ```c
   void do_deq(struct jobcmd deqcmd)
   {
   	/* other codes */
   	if (current && current->job->jid == deqid) {
   		/* other codes*/
   	} else {  /* maybe in waitqueue, search it */
   		select = NULL;
   		selectprev = NULL;
   		if (head) {
   			/* other codes*/		
   			if (select == selectprev)	head = NULL; // same logic flaw here
   		}
           /* other codes*/
   	}
   }
   ```

   Many lines of the code have been modified to fix this bug and at the same time to improve readability. Basically, the modification is to decide whether it is the case that there only exists one node or the case that the node with highest priority is the head of the linked list when `select == selectprev`.

   Modified codes are below. 

   ```c
   struct waitqueue* jobselect()
   {
   	/* other codes*/
       
   	if (head) 
   	{
   		if(head->next==NULL) // if only one node, pick the head
   		{
   			select = head;
   			head = NULL;
   		}
   		else
   		{
   			/* other codes*/
   
   			if(select==selectprev) // means selected one is head
   				head = head->next; // picked the head and thus replace it with its successor
   			/* other codes*/
   		}	
   	}
   	return select;
   }
   
   void do_deq(struct jobcmd deqcmd)
   {
   	/* other codes*/
   	
   	/* current jodid == deqid, terminate current job */
   	if (current && current->job->jid == deqid) {
   		/* other codes*/
   	} else {  /* maybe in waitqueue, search it */
   		select = NULL;
   		selectprev = NULL;
   		if (head) {
   			/* other codes*/	
   			
   			if (select == selectprev) //choosing head
   				head = head->next;				
   			// sew the linked list
   			selectprev->next = select->next;
   		}
   		/* other codes*/
   	}
   }
   ```

4. There is a bug that leads to cycles in the linked list, which is that previously, when taking a node out of the linked list, its `next` attribute still holds the pointer to its previous next node, and when switching jobs and putting `current` node into the linked list of jobs, the `next` attribute is still not cleared. Therefore, there is a chance that a node at the tail of the linked list points to a node in the same list, which leads to cycles. To fix this issue, we can clear the `next` attribute of selected node in `jobselect()`.

   ```c
   struct waitqueue* jobselect()
   {
   	/* other codes*/
   	if (head) 
   	{
   		if(head->next==NULL) // if only one node, pick the head
   		{
   			/* other codes*/
   		}
   		else
   		{
   			/* other codes*/
   			if(select==selectprev) // means selected one is head
   				head = head->next; // picked the head and thus replace it with its successor
   			selectprev->next = select->next; // sew the linked list
   			select->next = NULL; // avoid cycle HERE
   		}	
   	}
   	return select;
   }
   ```

5. Unintended output bug. After dequeuing a job, the next call of `jobswitch()` may output `begin start new job`, which should only be output when a brand new job start to run. The reason why this bug exists is that it is not only when a brand new job is run for the first time that the predicate `next!= NULL&& current == NULL` is fulfilled, but also when the `current` has been dequeued and `next=jobselect()` has been run.

   ```c
   // in jobswitch
   //before
   else if (next != NULL && current == NULL) {   /* start new job */
   	    printf("begin start new job\n");
   		current = next;
   		next = NULL;
   		current->job->state = RUNNING;
   		kill(current->job->pid, SIGCONT);
   		return;
   }
   //after
   else if (next != NULL && current == NULL) {   /* start new job */
   		if(next->job->run_time == 0)
   	    	printf("begin start new job\n");
   		current = next;
   		next = NULL;
   		current->job->state = RUNNING;
   		kill(current->job->pid, SIGCONT);
   		return;
   }
   ```

6. Sync bug. Inside `do_enq()`, after forking, if the parent process runs far ahead of the child process such that the parent process signals `SIGCONT` first then the child process raises `SIGSTOP`, this may cause synchronous problems that lead to child process running forever. 

   ```c
   //in do_enq()
   else {
   		// in parent process
   		waitpid(pid,NULL,0);// add this line
   		newjob->pid = pid;
   		printf("\nnew job: jid=%d, pid=%d\n", newjob->jid, newjob->pid);
   		
   }
   ```

   

## 11. Run the job scheduler program, And analyze the execution of the submitted job

1. After a job has been submitted, it is first placed in the FIFO queue.
2. At the next timer interrupt, `sig_handler()` is called and then inside `schedule()` is called, which will pull the job out of the FIFO queue.
3. Then according to the type of the command of the job, scheduler will do different things. Here, since a job is submitted, the type of the command is `ENQ` and `do_enq()` will be called.
4. Inside the `do_enq()`, a new instance of `jobinfor` is created and then a new `waitqueue` node, a job control block, is created.
5. The new node will be appended to the tail of the linked list of jobs.
6. A new process is forked and it stops immediately.

Then at some time(maybe right after its stop), scheduler calls `jobselect()`, and if inside the function, this job is selected to run in the next time slice, then the pointer `next` will be set pointing to the node of this job. Then

1. If the current job has done, free its space.

   Else, the current job is not done yet, then stop the job by signaling `SIGSTOP` to the current job.

2. Run this job by signaling `SIGCONT` to it.

Then if this job is run for the first time, it execute the instruction of `execv(arglist[0],arglist)`, cleaning user-space data and going to run its specific program. If the execution of `execv()` failed, it exits immediately.

After receiving `SIGCONT`, the job begin or continue to run until it finishes running or receives `SIGSTOP`.

## 12. Understand the process of job scheduling——Submit a new job (Execution results and corresponding code)

After running the program of `enq.c`, if the new job is the first(only) one in the scheduler’s job queue, then the scheduler will output the information of the job with an additional message stating `begin start new job`. An example is the below.

```
new job: jid=1, pid=28511
begin start new job
```

if the new job is not the only one in the the scheduler’s job queue, then the scheduler will only output the information of the job (for instance, the first line of the above example).

The corresponding code is below (listing called related functions in chronological order).

Simply, the order of called functions is `main()` in `enq.c`-> in `scheduler.c`, `sig_handler()` -> `schedule()` -> `do_enq()`-> `jobselect()` -> `jobswitch()`.

```c
// in enq.c, codes for submitting a job
int main(int argc,char *argv[])
{
	int	p = 0;
	int	fd;
	char	c, *offset;
	struct jobcmd enqcmd;
	if (argc == 1) {
		usage();
		return 1;
	}
	
	while (--argc > 0 && (*++argv)[0] == '-') {
		while ((c = *++argv[0]))
			switch (c) {
			case 'p':
				p = atoi(*(++argv));
				argc--;
				break;
			default:
				printf("Illegal option %c\n",c);
				return 1;
			}
	}
	
	if (p < 0 || p > 3) {
		printf("invalid priority: must between 0 and 3\n");
		return 1;
	}
	enqcmd.type = ENQ;
	enqcmd.defpri = p;
	enqcmd.owner = getuid();
	enqcmd.argnum = argc;
	offset = enqcmd.data;
	// ":".join(argv)
	while (argc-- > 0) {
		strcpy(offset,*argv);
		strcat(offset,":");
		offset = offset + strlen(*argv) + 1;
		argv++;
	}
	if ((fd = open(FIFO,O_WRONLY)) < 0)
		error_sys("enq open fifo failed");
	
	if (write(fd,&enqcmd,DATALEN)< 0)
		error_sys("enq write failed");
	close(fd);
	return 0;
}
// in scheduler.c, codes for scheduling jobs
void sig_handler(int sig, siginfo_t *info, void *notused)
{
	int status;
	int ret;
	
	switch (sig) {
	case SIGVTALRM: // when the timer runs out
		schedule();
		return;
     /* other unrelated codes*/
	}
}
void schedule()
{
	struct jobinfo *newjob = NULL;
	struct jobcmd cmd;
	int count = 0;
	// DATALEN = sizeof(struct jobcmd)
	bzero(&cmd, DATALEN); // init cmd and clear space with zeros
	//read from fifo pipe and place data of DATALEN into cmd
	if ((count = read(fifo, &cmd, DATALEN)) < 0) // read from the command queue
		error_sys("read fifo failed");
		
	switch (cmd.type) {
	case ENQ:
		do_enq(newjob,cmd);
		break;
	/* other unrelated codes*/
	}
	/* Update jobs in waitqueue */
	updateall();
	/* select the highest priority job to run */
	next = jobselect();
	jobswitch();
}
void do_enq(struct jobinfo *newjob, struct jobcmd enqcmd)
{
	int		i=0, pid;
	char	*offset, *argvec, *q;
	char	**arglist;
	sigset_t	zeromask;
	sigemptyset(&zeromask);
	/* fill jobinfo struct */
	newjob = (struct jobinfo *)malloc(sizeof(struct jobinfo));
	newjob->jid = allocjid();
	newjob->defpri = enqcmd.defpri;
	newjob->curpri = enqcmd.defpri;
	newjob->ownerid = enqcmd.owner;
	newjob->state = READY;
	newjob->create_time = time(NULL);
	newjob->wait_time = 0;
	newjob->run_time = 0;
	arglist = (char**)malloc(sizeof(char*)*(enqcmd.argnum+1));
	newjob->cmdarg = arglist;
	offset = enqcmd.data;
	argvec = enqcmd.data;
	while (i < enqcmd.argnum) {
		// split data by ':' into array of string
		if (*offset == ':') {
			*offset++ = '\0';//////
			q = (char*)malloc(offset - argvec);
			strcpy(q,argvec);
			arglist[i++] = q;
			argvec = offset;
		} else
			offset++;
	}
	arglist[i] = NULL;	
	/* add new job to the queue */
	struct	waitqueue *newnode, *p;
	newnode = (struct waitqueue*)malloc(sizeof(struct waitqueue));
	newnode->next = NULL;
	newnode->job = newjob;
	if (head) {
		// find the tail
		for (p = head; p->next != NULL; p = p->next);
		p->next = newnode;
	} else
		head = newnode;
	/* create process for the job */
	if ((pid = fork()) < 0)
		error_sys("enq fork failed");
	/* In child process */
	if (pid == 0) {
		newjob->pid = getpid();
		/* block the child wait for run */
		raise(SIGSTOP);		
		/* dup the globalfile descriptor to stdout */
		dup2(globalfd,1);
		if (execv(arglist[0],arglist) < 0)
			printf("exec failed\n");
		exit(1);
	} else {
		// in parent process
		newjob->pid = pid;
		printf("\nnew job: jid=%d, pid=%d\n", newjob->jid, newjob->pid);
	}
}

void jobswitch()
{
	struct waitqueue *p;
	int i;
	
	if (current && current->job->state == DONE) {           /* current job finished */
		
		/* job has been done, remove it */
		for (i = 0; (current->job->cmdarg)[i] != NULL; i++) {
			free((current->job->cmdarg)[i]);
			(current->job->cmdarg)[i] = NULL;
		}

		free(current->job->cmdarg);
		free(current->job);
		free(current);
		
		current = NULL;
	}
	
	if (next == NULL && current == NULL)          /* no job to run */
		return;
	else if (next != NULL && current == NULL) 
	{   /* start new job */
		
	    printf("begin start new job\n");
		current = next;
		next = NULL;
		current->job->state = RUNNING;
		kill(current->job->pid, SIGCONT);
		return;
	} else if (next != NULL && current != NULL) 
	{ /* do switch */
		
		kill(current->job->pid, SIGSTOP);
		current->job->curpri = current->job->defpri;
		current->job->wait_time = 0;
		current->job->state = READY;
		
		/* move back to the queue */
		
		if (head) {
			for (p = head; p->next != NULL; p = p->next);
			p->next = current;
		} else {
			head = current;
		}
		
		current = next;
		next = NULL;
		current->job->state = RUNNING;
		kill(current->job->pid, SIGCONT);
		return;
	} else {    /* next == NULL && current != NULL, no switch */
		return;		
	}
}
```

## 13. Understand the process of job scheduling——End of job execution (Execution results and corresponding code)

After the end of a job, the process of that job will signal a `SIGCHLD` to its parent process, which is the process of the scheduler. If the process of that job exited normally, then the scheduler will output a message like `"normal termation, exit status = 0 jid = 1, pid = 1234"`.

The process is that once the scheduler received the signal `SIGCHLD` from the process of a job, `sig_handler()` will be called in response. Inside the function, the scheduler will first wait for the child process to finish and then check the status of that process and output messages accordingly.

The corresponding code is the below

```c
void sig_handler(int sig, siginfo_t *info, void *notused)
{
	int status;
	int ret;
	switch (sig) {
	/* unrelated branch */
	case SIGCHLD:
		ret = waitpid(-1, &status, WNOHANG);
		if (ret == 0 || ret == -1)
			return;
		// if exits normally using exit()
		if (WIFEXITED(status)) {
			current->job->state = DONE;
			printf("normal termation, exit status = %d\tjid = %d, pid = %d\n\n",
				WEXITSTATUS(status), current->job->jid, current->job->pid);
		}  
		// else if the child process terminated due to unhandled signal
		else if (WIFSIGNALED(status)) {
		    printf("abnormal termation, signal number = %d\tjid = %d, pid = %d\n\n",
				WTERMSIG(status), current->job->jid, current->job->pid);
		}
		// else if the child process has been stopped 
		else if (WIFSTOPPED(status)) {
		    printf("child stopped, signal number = %d\tjid = %d, pid = %d\n\n",
				WSTOPSIG(status), current->job->jid, current->job->pid);
		}
		return;
	/* unrelated branch */
	}
}
```

## 14. Understand the process of job scheduling——job scheduling due to Priority(Execution results and corresponding code)

It is in `updateall()` that the priorities of jobs are updated based on waiting time and the current priorities of jobs. Since every 100 milliseconds that jobs waits will raise the priorities of waiting jobs by 1 and `schedule()` will be called every 100ms, in `updateall()`, we simply add 1 to the priories of waiting jobs at the same time of preventing priories to overflow.

In `jobselect()`, jobs are selected firstly based on their priorities then their waiting times. If two jobs have the same priorities, then the one that waits longer will be selected. 

The corresponding code is the below.

```c
void schedule()
{
	/* other unrelated codes*/
	updateall();
	/* select the highest priority job to run */
	next = jobselect();
	/* stop current job, run next job */
	jobswitch();
}
void updateall()
{
	struct waitqueue *p;	
	/* update running job's run_time */
	if (current)
		current->job->run_time += 1;   
	/* update ready job's wait_time */
	for (p = head; p != NULL; p = p->next) {
		p->job->wait_time += 1;
		int curprior = p->job->curpri;
		if(curprior<3){
			curprior++;
			p->job->curpri = curprior;
		}
	}
}

struct waitqueue* jobselect()
{
	struct waitqueue *p, *prev, *select, *selectprev;
	int highest = -1;
	select = NULL;
	selectprev = NULL;
	if (head) 
	{
		if(head->next==NULL) // if only one node, pick the head
		{
			select = head;
			head = NULL;
		}
		else
		{
			for (prev = head, p = head; p != NULL; prev = p, p = p->next) 
			{
				//pick the job of higher priority or longer waiting time
				if (p->job->curpri >= highest) 
				{
					if((p->job->curpri > highest) || (p->job->wait_time > select->job->wait_time))
					{
						select = p;
						selectprev = prev;
						highest = p->job->curpri;
					}
				}
			}
			if(select==selectprev) // means selected one is head
				head = head->next; // picked the head and thus replace it with its successor

			selectprev->next = select->next; // sew the linked list
			select->next = NULL; // avoid cycle
		}	
	}
	return select;
}
```



## 15. Understand the process of job scheduling——Job scheduling due to time slice (Execution results and corresponding code)

It is in the `main()` of `scheduler.c` that a timer for interruption is set. When the timer goes off periodically (every 100ms), `SIGVTALRM` is signaled to the scheduler and then `sig_handler()` will be called. Inside the `sig_handler()`, `schedule()` is called to manage jobs accordingly. In the end of `schedule()`, `jobswitch()` is called to switch jobs to run and put jobs into the queue.

So the time slice for a job to run continuously is 100ms.

The related code is below. 

```c
int main()
{
	struct timeval interval;
	struct itimerval new,old;
	struct stat statbuf;
	struct sigaction newact,oldact1,oldact2;
	/* unrelated codes */
    
	/* setup signal handler */
	newact.sa_sigaction = sig_handler;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = SA_SIGINFO;
	//binding sig handler
	sigaction(SIGCHLD,&newact,&oldact1);
	sigaction(SIGVTALRM,&newact,&oldact2);
	
	/* timer interval: 0s, 100ms */
	interval.tv_sec = 0;
	interval.tv_usec = 100*1000;//microseconds-> 100ms
	
	new.it_interval = interval;
	new.it_value = interval;
	setitimer(ITIMER_VIRTUAL,&new,&old);

	printf("OK! Scheduler is starting now!!\n");
	
	while (siginfo == 1);
	
	close(fifo);
	close(globalfd);
	return 0;
}

void sig_handler(int sig, siginfo_t *info, void *notused)
{
	int status;
	int ret;
	
	switch (sig) {
	case SIGVTALRM: // when the timer runs out
		schedule();
		return;
            
     /*
     unrelated codes
     */
	}
}

void schedule()
{
	/*
	unrelated codes
	*/
	
	/* stop current job, run next job */
	jobswitch();
}

void jobswitch()
{
	struct waitqueue *p;
	int i;
	
	if (current && current->job->state == DONE) {           /* current job finished */
		
		/* job has been done, remove it */
		for (i = 0; (current->job->cmdarg)[i] != NULL; i++) {
			free((current->job->cmdarg)[i]);
			(current->job->cmdarg)[i] = NULL;
		}

		free(current->job->cmdarg);
		free(current->job);
		free(current);
		
		current = NULL;
	}
	
	if (next == NULL && current == NULL)          /* no job to run */
		return;
	else if (next != NULL && current == NULL) 
	{   /* start new job */
		
	    printf("begin start new job\n");
		current = next;
		next = NULL;
		current->job->state = RUNNING;
		kill(current->job->pid, SIGCONT);
		return;
	} else if (next != NULL && current != NULL) 
	{ /* do switch */
		
		kill(current->job->pid, SIGSTOP);
		current->job->curpri = current->job->defpri;
		current->job->wait_time = 0;
		current->job->state = READY;
		
		/* move back to the queue */
		
		if (head) {
			for (p = head; p->next != NULL; p = p->next);
			p->next = current;
		} else {
			head = current;
		}
		
		current = next;
		next = NULL;
		current->job->state = RUNNING;
		kill(current->job->pid, SIGCONT);
		return;
	} else {    /* next == NULL && current != NULL, no switch */
		return;		
	}
}
```

