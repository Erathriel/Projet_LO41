/*-----------------------------------------------------------
                  recouvrement
-------------------------------------------------------------*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void attributsProcessus(const char *quand)
{
    printf("\n%s\n", quand);
    printf("--------------------------------\n");
    printf("Numero du processus      : %d\n", getpid());
    printf("Numero du processus pere : %d\n", getppid());
    printf("Proprietaire             : %d\n", getuid());
}

int main(void)
{
    attributsProcessus("AVANT recouvrement");
    fflush(stdout);		/* cf remarque 1 */
    execl("./prg3_apres", "prg3_apres", "patate", NULL);
    printf("ne passe pas ici\n");
    return 0;
}

/*-------------------------------------------------------------- 
   REMARQUE 1:
   
      meme chose que remarque 2 dans prg3.c
      fflush prudent car stdout peut etre redirige, auquel cas les
      tampons ne sont pas vides et les sorties du premier
      appel de attributsProcessus sont ecrasees
-------------------------------------------------------------*/
