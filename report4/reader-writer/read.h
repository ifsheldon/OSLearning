#include"init.h"
//given two sem db, rc
void *reader(int *buffer){
    // Reader should read the value of buffer
    // Reader cannot start to work when there is any writer working
    // You should implement this using semaphore

    // You should output this just after this thread gets semaphore
    sem_wait(&rc);
    printf("Reader gets sem\n");
    readcount++;
    if(readcount==1) // for the first reader of successive readers, block writers
        sem_wait(&db);     
    sem_post(&rc);

    sem_wait(&rc);
    readcount--;
    // you should output this just before this thread releases the semaphore
    printf("Reader reads %d and releases\n", *buffer);
    if(readcount==0)// for the first reader of successive readers, unblock writers
        sem_post(&db);
    
    sem_post(&rc);
}

