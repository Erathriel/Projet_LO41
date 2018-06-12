/*-----------------------------------------------------------
                Perte de signaux

        "le fils envoie NbaEnvoyer signaux au pere"

        -> on introduit un accuse de reception

-------------------------------------------------------------*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


/* h un sert dans chaque processus, il faudrait sinon creer */
/* un autre fichier pour le fils et faire un exec */

#define NBaEnvoyer 100
#define NB_SIG SIGRTMIN

int nbEnvoye = 0, nbRecu = 0;
int pidfils;

void erreur(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void traitantPere(int num)
{
    nbRecu++;
    printf("Pere Nb recus : %d\n", nbRecu);
    kill(pidfils, SIGUSR1);	/* accuse de reception */
}

void traitantFils(int num)
{

    nbEnvoye++;
    printf("Fils traitant Nb Envoyé : %d\n", nbEnvoye);
    /* accuse de reception */

}				/* indispensable */

int main(int argc, char *argv[])
{
    int res, j;
    sigset_t courant,pendant;

    /* --------------- On bloque tout------------ */

    sigfillset(&courant);
    if (sigprocmask(SIG_SETMASK, &courant, NULL) < 0) erreur ("Pb de sigprocmask"); ;
    /*sleep(5); */
    /*-----------On verifie le masque eventuellement----------*/
    sigpending(&pendant);
    for (j = 1; j < NB_SIG; j++)
    {
        if (sigismember(&pendant, j))
            fprintf(stdout, "en Attente de %d (%s)\n", j, sys_siglist[j]);
        /*fprintf(stdout, "\t en Attente de %d \n", j); */

    }
    sigemptyset(&courant);

    /* --------------le programme fait son boulot */

    res = fork();
    if (res > 0)  		/* pere */
    {
        signal(SIGUSR1, traitantPere);
        pidfils = res;
        sigprocmask(SIG_SETMASK, &courant, NULL);	/* on peut recevoir les IT */
        while (wait(NULL) == -1);
        printf("-- fin du pere : %d signaux recus\n", nbRecu);
    }
    else if (res == 0)  	/* fils   */
    {
        /*signal(SIGUSR1, traitantFils);*/
        sigprocmask(SIG_SETMASK, &courant, NULL);	/* on peut recevoir les IT */

        /*              on minimise volontairement le laps de temps */
        /*              entre kill et pause  */

        while (nbEnvoye < NBaEnvoyer)
        {
            int i;
            nbEnvoye++;
            kill(getppid(), SIGUSR1);
            for (i = 0; i < 100000; i++);	/* pour mettre en evidence interblocage a coup sur */
            printf ("fils attente signal\n") ;
            pause();	/* attend accuse de reception */
            printf("Fils Nb envoyes : %d\n", nbEnvoye);
        }
        printf("-- fin du fils : %d signaux envoyes\n", nbEnvoye);
    }
    return 0;
}
