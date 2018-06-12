/* Fichier pv.h */

/****************************************************************************************/
/* 	Operation sur les semaphores					      								*/	
/*								      													*/
/*	struct sembuf {					      												*/ 	
/*	unsigned short sem_num // nmr de semaphore		      								*/
/*	short sem_op // opération sur le semaphore		      								*/
/*	short sem_flag // options				      										*/
/*				}																		*/
/*	Pour sem_op la valeur est interprétée comme suit :									*/
/*	>0 la valeur du semaphor est augmentée de la valeur correspondante					*/
/*	    tout les processus en attente d'une augmentation sont réveillé					*/
/*	= 0 Teste si la semphore a la valeur 0. Dans le cas contraire, le processus			*/
/*		est mis en attente de la mise à zéro											*/
/*	<0 la valeur absolue est retranchée du sémaphore									*/	
/*																						*/
/*			      																		*/
/*								      													*/
/*	int semop (int semid,struct sembuf(*sops)[],int nbops)	      						*/
/*	semid : identificateur de l'ensemble des semaphores	      							*/
/*	sops: pointeur vers un tableau de nbops structure de type sembuf					*/
/*							      														*/						
/****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SEMPERM 0600

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

int	Pn (int SemId, int n, int Nsem) { 	/*   P(n,s)     */
	struct sembuf SemBuf ;
	SemBuf.sem_num = Nsem ;
	SemBuf.sem_op = -n ;
	SemBuf.sem_flg = 0 ;
	return semop(SemId, &SemBuf,1) ;
}

int	Vn (int SemId, int n, int Nsem) { 	/*   V(n,s)     */
	struct sembuf SemBuf ;
	SemBuf.sem_num = Nsem ;
	SemBuf.sem_op = n ;
	SemBuf.sem_flg = 0 ;
	return semop(SemId, &SemBuf,1) ;
}

int	Pp (int SemId, int Nsem1, int Nsem2) {	/*   P(s1,s2)     */
	struct sembuf SemBuf[2]={{0,-1,0},{0,-1,0}} ;
	SemBuf[0].sem_num = Nsem1 ;
	SemBuf[1].sem_num = Nsem2 ;
	return semop (SemId, SemBuf,2) ;
}
