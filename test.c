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
#define NB_ASCENCEUR 3

FILE* debugFile=NULL;

// Structure definissant un ascenseur
typedef struct Ascenseur {
	int id;
	int capacite;
	int etageCourant;
	int etageCible[CAPACITE_TOTAL];
	int etageDepart;
	int etat; // 0 = veille, 1 = en marche, 2 = a l arret a un etage
	bool utilisable;
} Ascenseur;

// Structure definissant la borne
typedef struct Borne
{
	
} Borne;

// Initialisation d'un ascenceur
static void ascenseurInit(Ascenseur *this,int id)
{
	this->id=id;
	this->capacite=0;
	this->etageCourant=0;
	this->etageCible[0]=0;
	this->etageDepart=0;
	this->etat=0;
	this->utilisable=true;
}

// Fonction executant les actions de chaque thread
void *thread(void *asc){

	Ascenseur *ascenseur = (Ascenseur *) asc;

	ascenseur->capacite=5;
	ascenseur->etageCourant=5;
	ascenseur->etageDepart=ascenseur->etageCourant;
	ascenseur->etageCible=2;
	ascenseur->etat=0;
	ascenseur->utilisable=true;
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
	
	pthread_t thread[NB_ASCENCEUR];
	Ascenseur asc[NB_ASCENCEUR];

	for(int i=0; i<NB_ASCENCEUR; i++)
	{
		ascenseurInit(asc[i],i);
		asc[i].id=i+1;
		createThread(&thread[i], &asc[i]);
	}

	//goTo(asc);
	debugFile = fopen("log.txt", "w");
	for (int k = 0; k < NB_ASCENCEUR; k++)
	{
		if (pthread_join(thread[k],NULL))
		{
			perror("pthread_join");
			return EXIT_FAILURE;

		} else {

			
			fprintf(debugFile, "On est dans le thread : %d\n", k );
			fprintf(debugFile, "id : %d\n",asc[k].id);
			fprintf(debugFile,"capa : %d\n", asc[k].capacite);
			fprintf(debugFile,"etage courant : %d\n", asc[k].etageCourant);
			fprintf(debugFile,"etage de depart : %d\n", asc[k].etageDepart);
			fprintf(debugFile,"etage cible : %d\n", asc[k].etageCible);
			fprintf(debugFile,"etat : %d\n", asc[k].etat);
			fprintf(debugFile,"utilisable : %d\n", asc[k].utilisable);
			

		}
	}
	fclose(debugFile);
	return 0;
}