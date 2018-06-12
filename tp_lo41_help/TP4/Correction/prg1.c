#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void erreur(const char* msg) {

  fprintf(stderr,"%s\n",msg);

  }

void traitantSIGINT(int num) {

  if (num!=SIGINT)
    erreur("Pb sur SigInt...");
  printf("\n---- Ctrl-C détecté ! Arrêt... -----\n");
kill(getpid(),9);
  }

void traitantSIGTSTP(int num) {
	printf("\n---- Ctrl-Z détecté ! Suspension du processus... -----\n");
	signal(SIGTSTP, SIG_DFL);
  }


int main(int argc,char* argv[]) {

    int s,i;
	struct sigaction sigTstpAction;
	sigemptyset(&sigTstpAction.sa_mask);
	sigTstpAction.sa_handler=traitantSIGTSTP;
	sigaction(SIGTSTP,&sigTstpAction,NULL);
    if (argc-1 != 1) {
        fprintf(stderr,"Appel : %s <nb de secondes>\n",argv[1]);
        return 1;
        }
    s=atoi(argv[1]);
    signal(SIGINT,traitantSIGINT);
    for (i=1;i<=s;i++) {
        sleep(1);
        printf("\r %d secondes ecoules...",i);
        fflush(stdout);               /* force affichage */
    }
    printf("\n");
    return 0;
}

