/**********************************************************************/
/* Created by Geoffrey NORO                                           */
/* Name : ascenceur.c                                                 */
/* Date : 08/06/2018                                                  */
/**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NB_ETAGES 25
#define CAPACITE_TOTAL 10

FILE* debugFile=NULL;

// Structure definissant un ascenseur
typedef struct Ascenseur {
	int id;
	int capacite;
	int etageCourant;
	int etageCible;
	int etageDepart;
	int etat; // 0 = veille, 1 = en marche, 2 = a l arret a un etage
	bool utilisable;
} Ascenseur;

// Structure definissant la borne
typedef struct Borne
{
	
} Borne;

// Fonction executant les actions de chaque thread
void *thread(void *arg){
	
}

// Fonction de creation de thread avec un ascenceur en argument
int createThread(pthread_t *t, Ascenseur *asc){

	int ret = pthread_create(t, NULL, thread, (void *) asc);
	if( ret == -1) {
		perror("pthread_create error");
		return EXIT_FAILURE;
	}
	return ret;

}

// Fonction permettant le deplacement de l'ascseur de haut en bas

void goTo(Ascenseur asc){
	debugFile = fopen("log.txt", "w+");
	//Cas ou l'ascenseur est utilisable
	if (asc.utilisable)
	{
		// Fais monter l'ascenseur
		if (asc.etageDepart<asc.etageCible && asc.etageCible<NB_ETAGES)
		{
			for (int i = asc.etageDepart; i < asc.etageCible; ++i)
			{
				asc.etat=1;
				asc.etageCourant++;
				asc.etat=2;
				if (asc.etat==2)
				{
					fprintf(debugFile,"Arret a l'etage : %d\n", asc.etageCourant);
					wait(100000);
					fprintf(debugFile,"Reprise de chemin\n");
				}
				fprintf(debugFile,"Etage : %d\n",asc.etageCourant);
			}
		} 
		// Fais descendre l'ascenseur
		else if (asc.etageDepart>asc.etageCible && asc.etageCible<NB_ETAGES)
		{
			for (int i = asc.etageDepart; i > asc.etageCible; --i)
			{
				asc.etat=1;
				asc.etageCourant--;
				asc.etat=2;
				if (asc.etat==2)
				{
					fprintf(debugFile,"Arret a l'etage : %d\n", asc.etageCourant);
					wait(100000);
					fprintf(debugFile,"Reprise de chemin\n");
				}
				fprintf(debugFile,"Etage : %d\n",asc.etageCourant);
			}
		} else{
			fprintf(debugFile,"Erreur : etage cible ou depart non repertorie...\n");
		}
	}
	// Cas ou l'ascenseur est en panne
	else {
		fprintf(debugFile,"Error : Ascenseur Inutilisable\n");
	}
	fclose(debugFile);
}

int main(int argc, char const *argv[])
{
	
	pthread_t thread[3];
	Ascenseur asc;

	for(int i=0; i<3; i++)
	{
		createThread(&thread[i], &asc);
	}

	asc.id=1;
	asc.capacite=5;
	asc.etageCourant=5;
	asc.etageDepart=asc.etageCourant;
	asc.etageCible=2;
	asc.etat=0;
	asc.utilisable=false;

	goTo(asc);

	debugFile = fopen("log.txt", "a");
	fprintf(debugFile, "%d",asc.id);
	fprintf(debugFile,"%d\n", asc.capacite);
	fprintf(debugFile,"%d\n", asc.etageCourant);
	fprintf(debugFile,"%d\n", asc.etageDepart);
	fprintf(debugFile,"%d\n", asc.etageCible);
	fprintf(debugFile,"%d\n", asc.etat);
	fprintf(debugFile,"%d\n", asc.utilisable);
	fclose(debugFile);
	return 0;
}
