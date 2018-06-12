#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


#define NB_SIG SIGRTMIN

void erreur(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void traitant(int num)
{
    
    printf("**---------Traitant Pere -----------------**\n");
    }


int main(int argc, char *argv[])
{
sigset_t courant,ancien;

sigemptyset(&courant);
signal(SIGINT,traitant);
sigaddset(&courant,SIGINT);
printf("Numéro de processus : %d \n",
	getpid());
if (sigprocmask(SIG_BLOCK, &courant, &ancien) < 0) erreur ("Pb de sigprocmask");
for(;;){
	sigsuspend(&courant);
	sigsuspend(&ancien);
	}
/* Ensemble des signaux que l'on bloque pas celui qu'on attend */
printf("Je viens de recevoir un signal \n");

return 0;


}
