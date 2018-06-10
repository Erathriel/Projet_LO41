/**********************************************************************/
/* Created by Geoffrey NORO                                           */
/* Name : ascenceur.c                                                 */
/* Date : 08/06/2018                                                  */
/**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define NB_ETAGES 25

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

// Fonction permettant le deplacement de l'ascseur de haut en bas

void goTo(Ascenseur asc){
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
					printf("Arret a l'etage : %d\n", asc.etageCourant);
					wait(100000);
					printf("Reprise de chemin\n");
				}
				printf("Etage : %d\n",asc.etageCourant);
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
					printf("Arret a l'etage : %d\n", asc.etageCourant);
					wait(100000);
					printf("Reprise de chemin\n");
				}
				printf("Etage : %d\n",asc.etageCourant);
			}
		} else{
			printf("Erreur : etage cible ou depart non repertorie...\n");
		}
	}
	// Cas ou l'ascenseur est en panne
	else {
		printf("Error : Ascenseur Inutilisable\n");
	}
}

int main(int argc, char const *argv[])
{
	FILE* debugFile=NULL;
	Ascenseur asc1;
	//Ascenseur asc2;
	//Ascenseur asc3;

	debugFile = fopen("log.txt", "w");

	asc1.id=1;
	asc1.capacite=5;
	asc1.etageCourant=5;
	asc1.etageDepart=asc1.etageCourant;
	asc1.etageCible=2;
	asc1.etat=0;
	asc1.utilisable=false;

	goTo(asc1);

	fprintf(debugFile, "%d",asc1.id);
	fprintf(debugFile,"%d\n", asc1.capacite);
	fprintf(debugFile,"%d\n", asc1.etageCourant);
	fprintf(debugFile,"%d\n", asc1.etageDepart);
	fprintf(debugFile,"%d\n", asc1.etageCible);
	fprintf(debugFile,"%d\n", asc1.etat);
	fprintf(debugFile,"%d\n", asc1.utilisable);
	fclose(debugFile);
	return 0;
}