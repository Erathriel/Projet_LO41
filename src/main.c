/**********************************************************************/
/* Created by Geoffrey NORO                                           */
/* Name : ascenceur.c                                                 */
/* Date : 08/06/2018                                                  */
/**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Ascenseur {
	int id;
	int capacite;
	int etageCourant;
	int etageCible;
	int etageDepart;
	int etat; // 0 = veille, 1 = en marche, 2 = a l arret a un etage
	bool utilisable;
} Ascenseur;

typedef struct Borne
{
	
} Borne;

int main(int argc, char const *argv[])
{
	Ascenseur asc1;
	//Ascenseur asc2;
	//Ascenseur asc3;

	asc1.id=1;
	asc1.capacite=5;
	asc1.etageDepart=0;
	asc1.etageCible=0;
	asc1.etageCourant=0;
	asc1.etat=0;
	asc1.utilisable=true;


	printf("%d\n", asc1.id);
	printf("%d\n", asc1.capacite);
	printf("%d\n", asc1.etageDepart);
	printf("%d\n", asc1.etageCible);
	printf("%d\n", asc1.etageCourant);
	printf("%d\n", asc1.etat);
	printf("%d\n", asc1.utilisable);
	return 0;
}