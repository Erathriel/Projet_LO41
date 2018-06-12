#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>


void *fonction_thread (void * arg) ; 

int main(void) {
	pthread_t thr;
	int nbre = 0;
	while ( pthread_create(&thr,NULL,fonction_thread,NULL) == 0) {
		printf("Thread %d crée\n", nbre); //on informe de la création du thread
		nbre ++ ;
		pthread_join(thr,NULL); //on attend la mort du thread avant de continuer
		//usleep(10000); //Si jamais on veut ralentir la création, on attend 10000ms
	}

	fprintf(stderr,"Echec de creation après %d \n",
		nbre-1);
	return EXIT_SUCCESS;
}
			
void *fonction_thread (void * arg) {
	return NULL ;

}  

