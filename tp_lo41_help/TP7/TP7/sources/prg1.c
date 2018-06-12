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
#include <pthread.h> 
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include "pv.h"

#define SKEY   (key_t) IPC_PRIVATE  



pid_t pid;
int shmid, i, sem_id;
int *p;
sem_t semaphore;


void erreurSystemeFin(const char* msg,int valeur_retour)
  {
  perror(msg);
  exit(valeur_retour);
  }



main()
{
  //sem_init(&semaphore, 0, 1);
  sem_id=initsem(SKEY);
  shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
  p = (int *)shmat(shmid, NULL, 0);
  *p = 0;	//initialisation � 0 du compteur

  switch (pid=fork()) {
     case (pid_t) -1 : erreurSystemeFin("",1);
     case (pid_t)  0 :   //processus fils
        //sem_wait(&semaphore);
      	for (i = 0; i < 1000000; i++) {
      		(*p)++;		
      		printf("Valeur dans le fils : %d\n", *p);
      	}
        V(sem_id,0);
        //sem_post(&semaphore);		
        exit(0);
     default:		//processus pere
        //sem_wait(&semaphore);
        P(sem_id,0);
      	for (i = 0; i < 1000000; i++) {
      		(*p)++;
      		printf("Valeur dans le pere : %d\n", *p);
      	}
        //sem_post(&semaphore);
     }
     
  wait(NULL);
  printf("Valeur finale de l'entier : %d\n", *p);
  shmctl(shmid, IPC_RMID, NULL);
  //sem_destroy(&semaphore);
  exit(0);

}

