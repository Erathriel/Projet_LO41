#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h> 
#include<sys/sem.h> 

/*********************************************************************/
/*  Pour Operation P et V 					     */

int sem_id;
struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */

void P(int semnum) {

sem_oper_P.sem_num = semnum;
sem_oper_P.sem_op  = -1 ;
sem_oper_P.sem_flg = 0 ;
semop(sem_id,&sem_oper_P,1);
}

void V(int semnum) {

sem_oper_V.sem_num = semnum;
sem_oper_V.sem_op  = 1 ;
sem_oper_V.sem_flg  = 0 ;
semop(sem_id,&sem_oper_V,1);
}

/*********************************************************************/
/* 

/*********************************************************************/

