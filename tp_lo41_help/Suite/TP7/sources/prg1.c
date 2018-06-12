/************************************************************/
/*  prg1.c : variable partag�e sans s�maphore               */
/*                                                          */
/*  Un processus et son fils partagent une variable enti�re */
/*  en m�moire partag�e. La variable, initialis�e � 0, est  */
/*  incr�ment�e 1000000 fois par chacun des deux processus. */
/*  La valeur finale de cette variable est affich�e par le  */
/*  processus p�re. Celui-ci s'est assur� que son fils      */
/*  �tait termin� (wait). Pourtant la  valeur affich�e est  */
/*  souvent diff�rente de 2000000 !                         */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE	
#define SEMPERM 0600

pid_t pid;
int shmid, i, sem_id;
int *p;

//Fonction permettant d'envoyer un message d'erreur
void erreurSystemeFin(const char* msg,int valeur_retour)
  {
  perror(msg);
  exit(valeur_retour);
  }

void main()
{
	sem_id=initsem(SKEY);
  shmid = shmget(IPC_PRIVATE, sizeof(int), 0666); //On cr�e un segment de m�moire partag� de la taille d'un entier avec les permissions de lecture/�criture � tout le monde (0666)
  p = (int *)shmat(shmid, NULL, 0); //On attache le segment de m�moire partag� cr�e, identifi� par shmid, � un segment de donn�es du processus, p prends l'adresse d'attachement du segment de m�moire partag�e
  *p = 0;	//initialisation � 0 du compteur

  switch (pid=fork()) {
     case (pid_t) -1 : erreurSystemeFin("",1);
     case (pid_t)  0 :   //processus fils
	for (i = 0; i < 1000000; i++) {
		(*p)++;	//incr�mentation du compteur
		printf("Valeur dans le fils : %d\n", *p);
	}
V(sem_id,0);
        exit(0);
     default:		//processus pere
P(sem_id,0);
	for (i = 0; i < 1000000; i++) {
		(*p)++; //incr�mentation du compteur
		printf("Valeur dans le pere : %d\n", *p);
	}
     }
     
  wait(NULL);
  printf("Valeur finale de l'entier : %d\n", *p);
  shmctl(shmid, IPC_RMID, NULL);
semctl(sem_id, 0, IPC_RMID, NULL);
  exit(0);

}

int initsem(key_t semkey) 
{
	int status = 0;		
	int semid_init;

   	union semun {
		int val;
		struct semid_ds *stat;
		ushort * array;
	} ctl_arg;

    if ((semid_init = semget(semkey, 2, IFLAGS)) > 0) {
		
	    	ushort array[2] = {0, 0};
	    	ctl_arg.array = array;
	    	status = semctl(semid_init, 0, SETALL, ctl_arg);
    }

   if (semid_init == -1 || status == -1) { 
	perror("Erreur initsem");
	return (-1);
    } else return (semid_init);
}

int	P (int SemId, int Nsem) {		/*   P(s)      */
	struct sembuf SemBuf = {0,-1,0} ;
	SemBuf.sem_num = Nsem ;
	return semop(SemId, &SemBuf,1) ;
}

int	V (int SemId, int Nsem) {		/*   V(s)      */
	struct sembuf SemBuf = {0,1,0} ;
	SemBuf.sem_num = Nsem ;
	return semop(SemId, &SemBuf, 1) ;
}
