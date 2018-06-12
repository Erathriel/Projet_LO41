/****************************************************************/
/*          Processus serveur : 				*/
/*	     - Crée la File de messages				*/
/*	     - Additionne les 2 nombres	recus dans chq message	*/
/*								*/
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct {
    long type;
    pid_t pidEmetteur;
    int nb_un;
    int nb_deux;
} trequeteClient;

typedef struct {
    long type;			/* Prend la valeur du PID client */
    int res;			/* Resultat de l'opération */
} treponse;

int main(int argc, char *argv[])
{

    int msgid;			/* valeur de retour de la creation de la file */
    key_t key;			/* valeur de retour de la creation de la clé */
    int longMSG ;	    
    trequeteClient requete;
    treponse reponse;


/****************************************************************/
/*  Creation de la clé IPC 					*/
/*  key_t ftok(const char *pathname, int proj_id);              */
/****************************************************************/

    if ((key = ftok(argv[0], 'A')) == -1) {
	perror("Erreur de creation de la clé \n");
	exit(1);
    }

/****************************************************************/
/* Creation de la file de message 				*/
/* int msgget(key_t key, int msgflg);				*/
/****************************************************************/

    if ((msgid = msgget(key, 0750 | IPC_CREAT | IPC_EXCL)) == -1) {
	perror("Erreur de creation de la file\n");
	exit(1);
    }

/* Lecture de la requête */
    while (1) {

/****************************************************************************************/
/* Reception de Requete							                */
/* int msgrcv (int idint, const void *msg,int longueur,long typemessage,int option)	*/
/****************************************************************************************/

	if ((longMSG =
	     msgrcv(msgid, &requete, sizeof(trequeteClient) - 4, 1,
		    0)) == -1) {
	    perror("Erreur de lecture requete \n");
	    exit(1);
	}

/* Construction du resultat */

	reponse.res = requete.nb_un + requete.nb_deux;
	reponse.type = requete.pidEmetteur;

/* type associé au message --> le pid du client */

/************************************************************************/
/* Envoi Reponse							*/
/* int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);	*/
/***********************************************************************/

	if (msgsnd(msgid, &reponse, sizeof(treponse) - 4,0) == -1) {
	    perror("Erreur de lecture requete \n");
	    exit(1);
	}

    }

/* A tester */  
 /* msgctl(msgid, IPC_RMID, NULL); */			
    return (0);
}
