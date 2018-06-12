// Modification 10/2014

/* Creation de 3 threads; chacune incremente un compteur qui lui est associe*/ 
    
/* Trois_Th.c */ 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int compteur[3];
 
/* fonction executee par  chaque thread */ 
/*void *fonc_thread(void *k) {

	int num = (int) k;
	if (num == 1)
	{
		printf("Je suis 1 : \n");
	}
	else if (num == 2){
		printf("Je suis 2\n");
	}
	else if (num == 0) {
		printf("Je suis 0 \n");
	}
	else {
		printf("erreur \n");
	}
	printf("Thread numero %d : mon tid est %d\n", 
		(int) k,pthread_self());
	for (;;)
	compteur[(int) k]++;

} 

int main(int argc, char *argv[]) {
	int i, num, a, b, c, d;
	pthread_t pth_id[3];
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	c = atoi(argv[3]);
	d = atoi(argv[4]);

	int arguments[3];

/* creation des threads */ 
/*	for (num = 0; num < 3; num++) {
		arguments[]
		pthread_create(pth_id + num, 0, fonc_thread, (void *) num);
		printf("Main: thread numero %d creee: id = %d\n", 
			num,pth_id[num]);
	} 
	usleep(10000);	/* attente de 10 ms */
	/* Affichage des compteurs (20 fois): entre 2 affichages, les compteurs peuvent avoir ete incrementes */ 
	/*printf("Affichage des compteurs\n");
	for (i = 0; i < 20; i++) {
		printf("%d \t%d \t%d\n", 
			compteur[0], compteur[1], compteur[2]);
		usleep(1000);		/* attente de 1 ms entre 2 affichages */
	/*}
	exit(0);
}*/

void *thread(void *arg){
	int *arguse = (int *) arg;
	int resultat;
	for (int i = 0; i < 3; ++i)
	{
		printf("%d\n", arguse[i]);
	}
	if (arguse[0] != 0)
	{
		resultat = arguse[1]*arguse[2];
		printf("%d\n", resultat);
	}

	pthread_exit(NULL);
}

int createThread(pthread_t *t, int *i){

	int ret = pthread_create(t, NULL, thread, (void*) i);
	
	if( ret == -1) {
		perror("pthread_create error");
		return EXIT_FAILURE;
	}
	return ret;

}


int main(int argc, char const *argv[])
{
	/*pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;*/
	pthread_t pthid[3];
	int a,b,c,d;
	int arg[3];
	for (int i = 0; i < 3; ++i)
	{
		if (i == 1)
		{
			arg[0]=i;
			arg[1]=a;
			arg[2]=b;
		}
		else if (i == 2) {
			arg[0]=i;
			arg[1]=a;
			arg[2]=b;
		}
		else if ( i == 0) {
			arg[0]=i;
			arg[1]=0;
			arg[2]=0;
		}
		else {
			printf("Erreur\n");
		}
		createThread(&pthid[i], arg);
	}

	return 0;
}


 
 
