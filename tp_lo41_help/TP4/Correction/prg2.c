
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define LGMAX 256

void erreur(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void traitant(int num)
{
    printf("---- passage par traitant ----\n");
}

void lectureChaine(int i)
{
    char tampon[LGMAX];
    int res, j;
    for (j = 0; j < LGMAX; j++)
	tampon[j] = ' ';	/* init */
    printf("lecture chaine %d ?\n", i);
    res = scanf("%s", tampon);
    printf("  -> res = %d, ligne %d = %s\n", res, i, tampon);
}

int main(int argc, char *argv[])
{
    
    signal(SIGINT, traitant);
    lectureChaine(1);
    lectureChaine(2);
    lectureChaine(3);

    return 0;
}
