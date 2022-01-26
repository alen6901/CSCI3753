#include "array.h"

int array_init(array *a) {                  // init the stack   
    a->array = malloc(ARRAY_SIZE*sizeof(int));
    a->top = 0;
    sem_init(&(a->empty), 0, ARRAY_SIZE-1);
    sem_init(&(a->full), 0, 0);
    sem_init(&(a->mutex), 0, 1);
    return 0;
}

int array_push(array *a, int element) {     // place element on the top of the stack
    sem_wait(&(a->empty));
    sem_wait(&(a->mutex));
    a->array[a->top++] = element;
    sem_post(&(a->mutex));
    sem_post(&(a->full)); 
    return 0;
}

int array_pop(array *a, int *element) {     // remove element from the top of the stack
    //sem_getvalue(&full, &value);
    //printf("\nvalue 1 = %d\n", value);
    sem_wait(&(a->full)); 
    //sem_getvalue(&full, &value);
    //printf("\nvalue 2 = %d\n",value);
    sem_wait(&(a->mutex));
    *element = a->array[--a->top];
    sem_post(&(a->mutex));
    sem_post(&(a->empty));
    return 0;
}

void array_free(array *a) {                 // free the stack's resources
    free(a->array);    
}

