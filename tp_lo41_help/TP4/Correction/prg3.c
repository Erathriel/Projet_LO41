#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void erreur(char* msg){

  fprintf(stderr,"%s\n",msg);
  exit(1);
}


int main(int argc,char* argv[]) {

  int s;
  sigset_t ens;
  printf("debut processus pere\n");

  if (argc-1 != 1) {
     fprintf(stderr,"Appel : %s <duree de vie en secondes>\n",argv[0]);
     exit(1);
     }
  s=atoi(argv[1]);
//Construction du masque
  sigemptyset(&ens);
  sigaddset(&ens,SIGINT);
//Installation du masque avant le fork, le fils héritera donc du masque
//sigprocmask(SIG_SETMASK,&ens,NULL);
  if (fork()==0) {
     printf("debut processus fils\n");
//Installation du masque dans le fils, on quittera seulement le père donc
//	sigprocmask(SIG_SETMASK,&ens,NULL);
     sleep(s);
     printf("fin du processus fils\n");
     }
  else {
//Installation du masque dans le père après le fork, le fils n'hérite pas du masque, donc on quittera seulement le fils
	sigprocmask(SIG_SETMASK,&ens,NULL);
	sleep(s);
    printf("fin du processus pere\n");
    }
return 0 ;
  }

  /* aléatoire :
  #include <time.h>
  // init srand(time(NULL));
  // nb_aleatoire = rand();

  // fonction rand entre a et b
  // int rand_a_b(int a, int b){
  //  return rand()%(b-a) + a;
  // }
  // RAND_MAX = 5
  // double frand_a_b(double a, double b){
  //   return (rand ()/(double)RAND_MAx) * (b-a) +a;
  // }

  */


