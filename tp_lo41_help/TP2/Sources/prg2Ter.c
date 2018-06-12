/*-----------------------------------------------------------
               Les tubes ordinaires
               le pere lit, le fils ecrit
-------------------------------------------------------------*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

void erreur(const char* msg)      {perror(msg);exit(1);}

#define LGMAX 100
  
main()
  {
  int pfd[2];
  char tampon[LGMAX];
  
  if (pipe(pfd) == -1) erreur("pipe");
  
  switch (fork()) {
     case -1 : erreur("fork");
     default  : {                /* pere */ 
               char *p = tampon;                    
               close(pfd[1]);
               while (read(pfd[0],p,1) != 0) p++;
               printf("chaine lue dans le tube : %s\n",tampon);
               break;
               }
     case 0 :                   /* fils */
               close(pfd[0]);
               sprintf(tampon,"%d",pfd[1]);  /* communique num desc */
               if (execl("./prg1TerFils","prg1TerFils",tampon,NULL)==-1)
                  erreur("execl");
               printf("ne passe jamais par ici car recouvrement\n");
     }
  }
               
               
