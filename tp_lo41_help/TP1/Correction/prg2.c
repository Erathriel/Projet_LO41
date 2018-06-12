/*-----------------------------------------------------------
    Creation de processus - Programme non deterministe
    
    Param : temps t en s
     t > 0 -> le processus pere attend t secondes avant de se terminer
     t < 0 ->              fils        -t
     t = 0 -> terminaison immediate des deux processus
-------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void erreurSystemeFin(const char *msg, int valeur_retour)
{
    perror(msg);
    exit(valeur_retour);
}

int main(int argc, char *argv[], char *arge[])
{
    pid_t pid;
    const int nbarg = 1;
    int temps_s;
    int status;

    temps_s = (argc - 1 == 1) ? atoi(argv[1]) : 0;

    switch (pid = fork()) {
    
    case (pid_t) - 1:
	erreurSystemeFin("", 1);	/* break inutile */
	
    case (pid_t) 0:
	/* on est dans le processus fils */
	printf("valeur de fork = %d \n", pid);
	if (temps_s < 0)
	    sleep(-temps_s);
	printf("je suis le processus fils %d de pere %d\n", getpid(),
	       getppid());
	printf("fin du processus fils\n");
	exit(0);		/* idem */
    
    default:
	/* on est dans le processus pere */
	while(wait(&status)!=-1)
{
	if (temps_s > 0)
	    sleep(temps_s);
	printf("valeur de fork = %d \n", pid);
	printf("je suis le processus pere %d et mon grand pere est : %d\n", getpid(),
	       getppid());
	printf("Le pid de mon fils mort est le : %d\n", pid); 
	printf("Le code retour de mon fils est : %d\n", status);
	printf("fin du processus pere\n");
	exit(0);

    }
}
return EXIT_SUCCESS;
}
