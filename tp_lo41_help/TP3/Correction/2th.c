#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#define NUM_THREADS 3

void erreur(const char *msg)
{
    perror(msg);
    exit(1);
}

void *AfficheEtat(void *data)
{

    long num;
    num = (long) data;
    printf("\nJe suis le thread #%ld \n", num);    
    printf("de TID : %ld \n", (long) pthread_self());
    fflush(stdout);
    pthread_exit(NULL);
}

int main(void)
{

    int i,j, rc;
    pthread_t thread_id[NUM_THREADS];
    // void * retour;
	
    for (i = 0; i < NUM_THREADS; i++) {
	if (rc =
	    pthread_create(&thread_id[i], 0, AfficheEtat, (void *) i) != 0)
	    erreur("Erreur Creation thread");
		pthread_join(thread_id[i],NULL); //On attend la fin du thread pour créer le suivant
	    //usleep(30000);  
    }
		    	 
	pthread_exit(NULL);
}
/*Dans l'exemple précédent, les threads sont créés en mode JOINABLE, c'est à dire que le processus qui a créé la thread attend la fin de celle-ci en restant bloqué sur l'appel à pthread_join. Lorsque la thread se termine, les ressources mémoire de la thread sont libérées grâce à l'appel à pthread_join. Si cet appel n'est pas effectué, la mémoire n'est pas libérée et il s'en suit une fuite de mémoire. Pour éviter un appel systématique à pthread_join (qui peut parfois être contraignant dans certaines applications), on peut créer la thread en mode DETACHED. Dans ce cas , la mémoire sera correctement libérée à la fin de la thread.

Pour se faire, il faut  ajouter:
        Pour cela il suffit d'ajouter le code suivant:*/

pthread_attr_t thread_attr;

if (pthread_attr_init (&thread_attr) != 0) {
fprintf (stderr, "pthread_attr_init error");
exit (1);
}

if (pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
fprintf (stderr, "pthread_attr_setdetachstate error");

exit (1);
}
puis de créer les threads avec des appels du type:

if (pthread_create (&th1, &thread_attr, thread1_process, NULL) < 0) {
fprintf (stderr, "pthread_create error for thread 1\n");
exit (1);
}
