#include "array.h"

void* threadProducer(void *args)
{
    for(int i = 0; i<ARRAY_SIZE; i++){
	array_push(*&args, rand() % 10);        // push random numbers until full
    }
}

void* threadConsumer(void *args)
{
    int i;
    for(int j = 0; j<ARRAY_SIZE; j++){
    	array_pop(*&args, &i); 
	printf("%d ", i); 			// pop and print until empty
    }
}


int main() {
    array my_array; 
    
    pthread_t *tidP;
    tidP = malloc(numPThreads*sizeof(pthread_t));
    pthread_t *tidC;
    tidC = malloc(numCThreads*sizeof(pthread_t));

    if (array_init(&my_array) < 0) exit(-1);                // init stack, exit if failed
    
    int err;

    for(int i = 0; i<numPThreads; i++){
        err = pthread_create(&(tidP[i]), NULL, threadProducer, &my_array);
	if (err != 0) printf("\ncan't create thread :[%s]", strerror(err));
        //else printf("\nThreadP created successfully\n");
    }

    for(int j = 0; j<numCThreads; j++){
    err = pthread_create(&(tidC[j]), NULL, threadConsumer, &my_array);
        if (err != 0) printf("\ncan't create thread :[%s]", strerror(err));
        //else printf("\nThreadC created successfully\n");
    }

    //printf("\nJoining threads\n");

    for(int k=0;k<numPThreads;k++){
        err = pthread_join(tidP[k],NULL);
	
    if (err != 0) printf("\ncan't join thread :[%s]", strerror(err));
    //else printf("\nThreadP joined successfully\n");
    }
    free(tidP);
    for(int l=0;l<numCThreads;l++){
        err = pthread_join(tidC[l],NULL);
        
    if (err != 0) printf("\ncan't join thread :[%s]", strerror(err));
    //else printf("\nThreadC joined successfully\n");
    }
    free(tidC);
    array_free(&my_array);                                  // destroy stack
    exit(0);
}
