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

Simply, jobs are a general concept that represent a unit of work or execution while a process is a concrete specification of a job, which has an actual implementation.

## 3. What are the differences between job scheduling and process scheduling?

The granularities of job scheduling and process scheduling differ. As the definition of jobs implies, a job may be concretized by a process group which has multiple processes. Therefore, sometimes, scheduling a job may imply scheduling multiple processes. 

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

## 5. How many statuses are in a job? And what are they?

本次实验作业有几种状态？请列举/////////////????

## 6. What programs for job control are used in this experiment? And their function?

## 7. What is used for inter-process communication in this experiment? And its function?

## 8. What should be noted when printing out the job name

## 9. Submit a job that takes more than 100 milliseconds to run (pleas paste your code)

## 10. List the bugs you found and your fix (Describe the cause of bugs in detail and how to modify it)

## 11. Run the job scheduler program, And analyze the execution of the submitted job

## 12. Understand the process of job scheduling——Submit a new job (Execution results and corresponding code)

## 13. Understand the process of job scheduling——End of job execution (Execution results and corresponding code)

## 14. Understand the process of job scheduling——job scheduling due to Priority(Execution results and corresponding code)

## 15. Understand the process of job scheduling——Job scheduling due to time slice (Execution results and corresponding code)

