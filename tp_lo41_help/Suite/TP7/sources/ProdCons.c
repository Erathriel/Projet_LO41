/* ProdCons.c */

#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "pv.h"

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE	
#define SEMPERM 0600

#define Snvide  0 
#define Snplein 1

int	ShmId;		/* descripteur du segment */
int 	SemId;		/* descripteur ensemble semaphores */
int   Ncases = 10;    /* nbr de cases du tampon */

void	Producteur(void) {
	int i, queue=0, MessProd;
	int *Tampon;
	/* attachement du segment */
	Tampon = (int *) shmat (ShmId, 0, 0);	
	
	
	srand(getpid());
	for(i=0; i<20; i++){
		sleep(rand()%3);   /* fabrique le message */
		MessProd = rand() % 10000;	
		printf("Product %d\n",MessProd);	
		P(SemId,Snvide);	
		Tampon[queue]=MessProd;	queue=(queue+1)%Ncases; 
		V(SemId,Snplein);	
	}
	/* detachement du segment */
	shmdt(0);
	exit(1);	
}
	
void	Consommateur(void) {
	int tete=0, MessCons, i;
	int *Tampon;

	/* attachement du segment */
	Tampon = (int*) shmat(ShmId, 0, SHM_RDONLY);
		

	srand(getpid());
	for(i=0; i<20; i++){
		P(SemId, Snplein);	
		MessCons = Tampon[tete]; tete=(tete+1)%Ncases;
		V(SemId,Snvide);
		printf("\t\tConsomm  %d\n",MessCons);	
		sleep(rand()%3);   /* traite le message */
	}	
	/* detachement du segment */
	shmdt(0);
exit(2);
}

int main(void) {
	int	NbSem = 2;	/* 2 semaphores*/
	pid_t	pid,p;	

	setvbuf(stdout,NULL,_IONBF,0);
	system("clear");

	/* creation du segment partage*/
	if ((ShmId = shmget (IPC_PRIVATE, 128, IPC_CREAT|IPC_EXCL|0644))<0)
		printf("Erreur Creation Segment");

	/* creation ensemble de semaphores */
	if ((SemId= semget(IPC_PRIVATE, NbSem, IPC_CREAT|0666)) < 0)
		printf("Erreur Creation semaphore");

	/* initialisation du semaphore Snvide a la valeur Ncases */
	semctl(SemId,Snvide ,SETVAL,Ncases);	
	/* initialisation du semaphore Snplein a la valeur 0 */
	semctl(SemId,Snplein ,SETVAL,0);	
	
	/* creation fils */
if ((p=fork())==0) Producteur();
if ((p=fork())==0) Consommateur();

	/* attente de terminaison */
	while (( pid = waitpid(-1,NULL ,0 )) > 0);

	/* suppression du segment */
	shmctl(ShmId,IPC_RMID,0);
	/* suppression des semaphores */
	semctl(SemId,1,IPC_RMID,0);
return (0);
}
