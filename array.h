#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_SIZE 1000                      // max elements in stack
#define numPThreads 5
#define numCThreads 5

#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>


typedef struct {
    int *array;  // storage array for integers
    int top;     // array index indicating where the top is
    sem_t empty; // semaphor for elements in queue
    sem_t full;  // semaphor for space in queue
    sem_t mutex; // binary semaphor to protect read/write
} array; 



int  array_init(array *a);                  // init the stack
int  array_push(array *a, int element);     // place element on the top of the stack
int  array_pop (array *a, int *element);    // remove element from the top of the stack
void array_free(array *a);                  // free the stack's resources
#endif
