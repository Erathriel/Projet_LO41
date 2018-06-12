/*-----------------------------------------------------------
                Perte de signaux sans synchro
                
         "le fils envoie NbaEnvoyer signaux au pere"
                 
      terminaison du fils : apres l'envoi des signaux
          "          pere : a la terminaison de son fils
-------------------------------------------------------------*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NBaEnvoyer 100

/* un seul sert dans chaque processus, il faudrait sinon creer */
/* un autre fichier pour le fils et faire un exec */

int nbEnvoye=0, nbRecu=0;  
                   

void traitantSIGUSR1(int num)
  {
  nbRecu++;
 /* exit(1);*/
 return;
  }
  
  
int main(int argc,char* argv[])
  {
  int res;

  /* ---------------installation du traitant */

  signal(SIGUSR1,traitantSIGUSR1);
  
  /* --------------le programme fait son boulot  */
  res=fork();
  if (res>0) {                 /* pere */
    
    while (wait(NULL)== -1);        /* wait interruptible */
    printf("-- fin du pere : %d signaux recus\n",nbRecu);
    }
  else if (res==0){                            /* fils */                     
    for (nbEnvoye=0;nbEnvoye<NBaEnvoyer;nbEnvoye++)
       kill(getppid(),SIGUSR1);
    printf("-- fin du fils : %d signaux envoyes\n",nbEnvoye);
    }
  return 0;
  }
  
