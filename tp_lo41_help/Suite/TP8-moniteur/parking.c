#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NB_VOITURE 20

int nbEntrant=0;
int nbSortant=0;
int nbVoiture=0;
int etrePasserelleOccupee=0;
pthread_mutex_t mutex;
pthread_cond_t attendre;

void demanderPassage()
{
	if(!etrePasserelleOccupee)
}

void seGarer()
{

}

void demanderSortir()
{

}

void quitterPasserelle()
{

}

