
#include <stdio.h>
#include <pthread.h>


/* définition du tampon */
#define N 10 /* Nb de cases du tampon */
#define NbMess 20 /* Nb de messages échangés */
int NbPleins=0;
int tete=0, queue=0;
int tampon[N];


pthread_t tid[2];

void Deposer(int m){
	
	
	if(NbPleins == N) 
	tampon[queue]=m;
	queue=(queue+1)%N;
	NbPleins++;
	

	
	
}
int Prelever(void){
	int m;
	

	if(NbPleins ==0) 
	m=tampon[tete];
	tete=(tete+1)%N;
	NbPleins--;
	
	
	
	return m;
}

/********** PRODUCTEUR */
void * Prod(void * k) {
	int i;
	int mess;
	srand(pthread_self());
	for(i=0;i<=NbMess; i++){
		usleep(rand()%10000); /* fabrication du message */
		mess=rand()%1000;
		Deposer(mess);
		printf("Mess depose: %d\n",mess);
	}
}

/********** CONSOMMATEUR */
void * Cons(void * k){
	int i;
	int mess;
	srand(pthread_self());
	for(i=0;i<=NbMess; i++){
		mess=Prelever();
		printf("\tMess preleve: %d\n",mess);
		usleep(rand()%1000000); /* traitement du message */
	}
}
void main() {
	int i, num;
	/* Initialisation des conditions et des mutex */	
	
	/* creation des threads */
	
	// attente de la fin des threads
	
	// libération des ressources
	
	
	return(0);
	}
