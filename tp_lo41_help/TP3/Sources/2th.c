#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#define NUM_THREADS	3

void erreur(const char *msg)
{
    perror(msg);
    exit(1);
}

void *AfficheEtat(void *data)
{

    long num;
    num = (long) data;
    printf("\nJe suis le thread #%ld \n", num);    
    printf("de TID : %ld \n", (long) pthread_self());
    fflush(stdout);
    pthread_exit(NULL);
}

int main(void)
{

    int i,j, rc;
    pthread_t thread_id[NUM_THREADS];
    // void * retour;
	
    for (i = 0; i < NUM_THREADS; i++) {
	if (rc =
	    pthread_create(&thread_id[i], 0, AfficheEtat, (void *) i) != 0)
	    erreur("Erreur Creation thread");
	    //usleep(30000);  
    }
		    	 
	pthread_exit(NULL);
}
